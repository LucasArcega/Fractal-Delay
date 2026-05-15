#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout FractalDelayAudioProcessor::createParameterLayout()
{
    // Converte valor dB para texto exibido no slider e no host
    auto valueToText = [](float value, int) -> juce::String
    {
        if (value <= silenceDb + 1.f)
            return "-inf dB";
        return juce::String(value, 1) + " dB";
    };

    // Converte texto digitado pelo utilizador de volta para dB
    // Aceita: "0", "-6.0", "-6.0 dB", "-inf", "-infinity"
    // Rejeita texto inválido devolvendo silenceDb (sem crash)
    auto textToValue = [](const juce::String& text) -> float
    {
        auto t = text.trim().toLowerCase();

        // Remove sufixo "db" se presente
        if (t.endsWith("db"))
            t = t.dropLastCharacters(2).trim();

        // Trata silêncio
        if (t == "-inf" || t == "-infinity" || t == "inf" || t.isEmpty())
            return silenceDb;

        // Tenta converter para número
        const float parsed = t.getFloatValue();

        // getFloatValue() devolve 0.0 para texto não numérico ("abc")
        // Distingue "0" válido de lixo verificando se o texto começa com dígito ou sinal
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
        juce::ParameterID{ "inputGainDb", 1 }, "Input", range, 0.f, attrs));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "outputGainDb", 1 }, "Output", range, 0.f, attrs));

    return layout;
}

FractalDelayAudioProcessor::FractalDelayAudioProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input",  juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , apvts(*this, nullptr, "PARAMS", createParameterLayout())
{
}

void FractalDelayAudioProcessor::prepareToPlay(double sampleRate, int)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    rmsSize  = static_cast<int>((1882.0 / 44100.0) * currentSampleRate);
    rmsCount = 0;
    peakIn   = 0.f;
    peakOut  = 0.f;
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

    buffer.applyGain(inGain);

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            peakIn = juce::jmax(peakIn, std::abs(buffer.getReadPointer(ch)[i]));
    }

    buffer.applyGain(outGain);

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            peakOut = juce::jmax(peakOut, std::abs(buffer.getReadPointer(ch)[i]));
    }

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

        msg.what = AudioToUIMessage::INCREMENT;
        msg.newValue = 0.f;
        audioToUI.push(msg);

        peakIn   = 0.f;
        peakOut  = 0.f;
        rmsCount = 0;
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
