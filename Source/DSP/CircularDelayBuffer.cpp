#include "DSP/CircularDelayBuffer.h"

void CircularDelayBuffer::prepare(int channels, int maxDelay, int marginSamples)
{
    // Pré-condições de programador: prepare só deve ser chamado com valores coerentes.
    jassert(channels > 0);
    jassert(maxDelay >= 0);
    jassert(marginSamples >= 0);

    numChannels     = channels;
    maxDelaySamples = maxDelay;

    // A margem dá folga para o maior bloco do host sem "comer" o fim do buffer ao ler atrasado.
    bufferLength = maxDelaySamples + juce::jmax(1, marginSamples);
    writeIndex   = 0;

    // setSize realoca só aqui; depois disso o processBlock não deve alocar de novo.
    data.setSize(numChannels, bufferLength, false, false, true);
}

void CircularDelayBuffer::reset() noexcept
{
    writeIndex = 0;
    // clear() zera todas as amostras — útil ao mudar a taxa de amostragem ou reabrir o projeto.
    data.clear();
}

float CircularDelayBuffer::readDelayed(int channel, int delayInSamples) const noexcept
{
    // Estado inválido: devolve silêncio em vez de ler fora dos limites do buffer.
    if (bufferLength <= 0 || numChannels <= 0)
        return 0.f;

    const int ch = juce::jlimit(0, numChannels - 1, channel);
    const int d  = juce::jlimit(0, maxDelaySamples, delayInSamples);

    // Contrato: atraso zero não lê o buffer; o bypass é responsabilidade de quem chama.
    if (d <= 0)
        return 0.f;

    // Índice circular: "d passos atrás" do writeIndex atual, antes da escrita desta amostra.
    const int readIndex = (writeIndex - d + bufferLength) % bufferLength;
    return data.getReadPointer(ch)[readIndex];
}

void CircularDelayBuffer::pushSample(int channel, float sample) noexcept
{
    if (bufferLength <= 0 || numChannels <= 0)
        return;

    const int ch = juce::jlimit(0, numChannels - 1, channel);

    // Grava na posição atual do anel; todos os canais compartilham o mesmo writeIndex por instante.
    data.getWritePointer(ch)[writeIndex] = sample;

    // Só avança o tempo do anel depois de termos escrito o último canal desta amostra.
    if (ch == numChannels - 1)
        writeIndex = (writeIndex + 1) % bufferLength;
}
