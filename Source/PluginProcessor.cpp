#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
constexpr float silenceDb = -80.f;

/** Teto de tempo de atraso reservado no buffer circular (segundos). Define o tamanho máximo do anel. */
constexpr double maxDelayTimeSeconds = 2.0;
} // namespace

juce::AudioProcessorValueTreeState::ParameterLayout FractalDelayAudioProcessor::createParameterLayout()
{
    // Converte valor em dB para o texto mostrado no controle e no host (DAW).
    auto valueToText = [](float value, int) -> juce::String
    {
        if (value <= silenceDb + 1.f)
            return "-inf dB";
        return juce::String(value, 1) + " dB";
    };

    // Converte o texto que o usuário digitou de volta para dB.
    // Aceita: "0", "-6.0", "-6.0 dB", "-inf", "-infinity"
    // Se o texto for inválido, devolve silenceDb (evita estado inválido ou exceção).
    auto textToValue = [](const juce::String& text) -> float
    {
        auto t = text.trim().toLowerCase();

        // Remove sufixo "db" se presente
        if (t.endsWith("db"))
            t = t.dropLastCharacters(2).trim();

        // Trata silêncio (-inf, etc.)
        if (t == "-inf" || t == "-infinity" || t == "inf" || t.isEmpty())
            return silenceDb;

        // Tenta converter para número
        const float parsed = t.getFloatValue();

        // getFloatValue() devolve 0.0 para texto não numérico ("abc").
        // Distingue zero válido de "lixo": o texto precisa começar com dígito ou sinal.
        const bool looksNumeric = t[0] == '-' || t[0] == '+' || juce::CharacterFunctions::isDigit(t[0]);
        if (!looksNumeric)
            return silenceDb;

        return juce::jlimit(-60.f, 12.f, parsed);
    };

    auto range = juce::NormalisableRange<float>(-60.f, 12.f, 0.1f, 0.45f);
    auto attrs = juce::AudioParameterFloatAttributes{}
                     .withStringFromValueFunction(valueToText)
                     .withValueFromStringFunction(textToValue);

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "inputGainDb", 1 }, juce::translate("Entrada"), range, 0.f, attrs));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "outputGainDb", 1 }, juce::translate("Saída"), range, 0.f, attrs));

    // Tempo de delay em milissegundos (0 = sem eco neste caminho; o buffer ainda é alimentado).
    // Sem controle deslizante no editor nesta fase — automação genérica do host / lista de parâmetros.
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "delayMs", 1 },
        "Delay (ms)",
        juce::NormalisableRange<float>(0.f, 2000.f, 1.f, 1.f),
        250.f));

    return layout;
}

FractalDelayAudioProcessor::FractalDelayAudioProcessor()
    : AudioProcessor(BusesProperties()
          .withInput(juce::translate("Entrada"), juce::AudioChannelSet::stereo(), true)
          .withOutput(juce::translate("Saída"), juce::AudioChannelSet::stereo(), true))
    , apvts(*this, nullptr, "PARAMS", createParameterLayout())
{
}

void FractalDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    rmsSize  = static_cast<int>((1882.0 / 44100.0) * currentSampleRate);
    rmsCount = 0;
    peakIn        = 0.f;
    peakOut       = 0.f;
    peakOutLeft  = 0.f;
    peakOutRight = 0.f;

    // Número de canais do bus principal (mono ou estéreo); o delay line segue o mesmo layout.
    const int numCh = juce::jmax(1, getTotalNumOutputChannels());

    // Quantas amostras cabem no máximo de 2 s de atraso (arredondado para a taxa de amostragem atual).
    const int maxDelaySamples =
        juce::roundToInt(currentSampleRate * maxDelayTimeSeconds);

    // Margem = pelo menos um bloco: evita ler "além" do que já foi escrito de forma segura.
    const int margin = juce::jmax(1, samplesPerBlock);
    delayLine.prepare(numCh, maxDelaySamples, margin);
    delayLine.reset();
}

bool FractalDelayAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& out = layouts.getMainOutputChannelSet();
    const auto& in  = layouts.getMainInputChannelSet();
    if (out != in) return false;
    return out == juce::AudioChannelSet::stereo() || out == juce::AudioChannelSet::mono();
}

void FractalDelayAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused(midi);
    juce::ScopedNoDenormals noDenormals;

    const float inGain  = juce::Decibels::decibelsToGain(
        apvts.getRawParameterValue("inputGainDb")->load());
    const float outGain = juce::Decibels::decibelsToGain(
        apvts.getRawParameterValue("outputGainDb")->load());
    // Parâmetro em ms; convertemos para amostras inteiras (v1 sem interpolação fracionária).
    const float delayMs = apvts.getRawParameterValue("delayMs")->load();

    buffer.applyGain(inGain);

    const int nCh = buffer.getNumChannels();
    const int nSm = buffer.getNumSamples();

    // Só precisamos de até 2 canais neste plugin; array fixo evita alocação no loop.
    float inScratch[2] {};

    for (int i = 0; i < nSm; ++i)
    {
        // Mesmo atraso para todos os canais neste bloco (leitura barata do APVTS).
        int delaySamples = juce::roundToInt(delayMs * static_cast<float>(currentSampleRate) / 1000.f);
        delaySamples = juce::jlimit(0, delayLine.getMaxDelaySamples(), delaySamples);

        // Copia entrada já com gain de entrada — é o que alimenta o delay e os medidores de entrada.
        for (int ch = 0; ch < nCh; ++ch)
            inScratch[ch] = buffer.getReadPointer(ch)[i];

        for (int ch = 0; ch < nCh; ++ch)
            peakIn = juce::jmax(peakIn, std::abs(inScratch[ch]));

        // 1) Lê o tap atrasado (ou bypass se atraso 0). 2) Depois empurra a entrada no anel.
        for (int ch = 0; ch < nCh; ++ch)
        {
            const float in = inScratch[ch];
            const float wet =
                (delaySamples <= 0) ? in : delayLine.readDelayed(ch, delaySamples);
            buffer.getWritePointer(ch)[i] = wet;
        }

        for (int ch = 0; ch < nCh; ++ch)
            delayLine.pushSample(ch, inScratch[ch]);
    }

    buffer.applyGain(outGain);

    // Picos de saída: medem o sinal já com delay e gain de saída (o que vai ao conversor D/A).
    if (nCh >= 1)
    {
        const float* p0 = buffer.getReadPointer(0);
        for (int i = 0; i < nSm; ++i)
            peakOutLeft = juce::jmax(peakOutLeft, std::abs(p0[i]));
    }
    if (nCh >= 2)
    {
        const float* p1 = buffer.getReadPointer(1);
        for (int i = 0; i < nSm; ++i)
            peakOutRight = juce::jmax(peakOutRight, std::abs(p1[i]));
    }
    else
    {
        // Mono: espelha L no R para os dois medidores do rodapé
        peakOutRight = peakOutLeft;
    }

    peakOut = juce::jmax(peakOutLeft, peakOutRight);

    rmsCount += buffer.getNumSamples();

    if (rmsCount >= rmsSize)
    {
        AudioToUIMessage msg;

        msg.what = AudioToUIMessage::PEAK_IN;
        msg.newValue = peakIn;
        audioToUI.push(msg);

        msg.what = AudioToUIMessage::PEAK_OUT;
        msg.newValue = peakOut;
        audioToUI.push(msg);

        msg.what = AudioToUIMessage::PEAK_OUT_LEFT;
        msg.newValue = peakOutLeft;
        audioToUI.push(msg);

        msg.what = AudioToUIMessage::PEAK_OUT_RIGHT;
        msg.newValue = peakOutRight;
        audioToUI.push(msg);

        msg.what = AudioToUIMessage::INCREMENT;
        msg.newValue = 0.f;
        audioToUI.push(msg);

        peakIn         = 0.f;
        peakOut        = 0.f;
        peakOutLeft    = 0.f;
        peakOutRight   = 0.f;
        rmsCount       = 0;
    }
}

juce::AudioProcessorEditor* FractalDelayAudioProcessor::createEditor()
{
    return new FractalDelayAudioProcessorEditor(*this);
}

void FractalDelayAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto state = apvts.copyState().createXml())
        copyXmlToBinary(*state, destData);
}

void FractalDelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FractalDelayAudioProcessor();
}
