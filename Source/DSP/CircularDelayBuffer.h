#pragma once

#include <JuceHeader.h>

/**
 * Linha de atraso circular (delay) por canal: amostras inteiras, sem interpolação (versão 1).
 *
 * Ideia geral: cada canal tem um buffer fixo; a cada instante gravamos a amostra atual e,
 * se quisermos eco, lemos uma posição alguns passos "para trás" no tempo (em amostras).
 *
 * Ordem por amostra de áudio (igual ao processBlock do plugin):
 * 1. readDelayed(canal, atrasoEmAmostras) — lê o valor gravado há @p atrasoEmAmostras passos
 *    em relação ao cursor de escrita **desta** amostra (ainda antes de gravar a nova).
 *    O atraso é limitado (clamp) ao intervalo [0, maxDelaySamples]. Se o atraso for 0,
 *    devolve 0.f; o processador usa isso para fazer bypass (saída = entrada) quando não há delay.
 * 2. pushSample(canal, amostra) — grava na posição atual do cursor. O índice de escrita só
 *    avança depois que o último canal (numChannels - 1) recebeu push na mesma amostra,
 *    para mono e estéreo ficarem alinhados no mesmo "relógio" de amostras.
 *
 * Depois de prepare(), não há alocações no caminho de áudio (requisito da thread de áudio).
 */
class CircularDelayBuffer
{
public:
    CircularDelayBuffer() = default;

    /** Redimensiona buffers e zera estado; é o único lugar onde há alocação de memória. */
    void prepare(int numChannels, int maxDelaySamples, int marginSamples);

    /** Zera o conteúdo do anel e volta o cursor de escrita ao início (silêncio completo). */
    void reset() noexcept;

    /**
     * Lê o "tap" atrasado; não move o ponteiro de escrita.
     * Ver comentário da classe: atraso 0 devolve 0.f de propósito.
     */
    float readDelayed(int channel, int delayInSamples) const noexcept;

    /**
     * Grava uma amostra no canal indicado.
     * Em estéreo: chame 0 depois 1; o writeIndex só incrementa após o canal 1.
     * Em mono: um único push já avança o índice.
     */
    void pushSample(int channel, float sample) noexcept;

    int getNumChannels() const noexcept { return numChannels; }
    int getMaxDelaySamples() const noexcept { return maxDelaySamples; }
    int getBufferLength() const noexcept { return bufferLength; }

private:
    /** Uma linha por canal, comprimento = bufferLength (atraso máximo + margem). */
    juce::AudioBuffer<float> data;

    /** Próxima posição onde todas as gravações da "rodada" atual vão escrever. */
    int writeIndex { 0 };

    /** Comprimento físico do anel (sempre >= atraso máximo lógico + margem). */
    int bufferLength { 0 };

    /** Maior atraso em amostras que o usuário ou a API pode pedir (clamp de leitura). */
    int maxDelaySamples { 0 };

    int numChannels { 0 };
};
