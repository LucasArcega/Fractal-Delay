#pragma once

#include "DSP/DspNumericGuards.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <optional>

/**
 * Conversões de tempo musical / ms → amostras (sem JUCE).
 * Tabela de divisões alinhada a Plans/fase-1-motor-delay-basico/03-sincronizacao-bpm-host.md
 */
namespace fractal_tempo
{

    /** Número de entradas em `beatsForDivisionIndex` / lista `delayDivision`. */
    inline constexpr int delayDivisionChoiceCount() noexcept { return 18; }

    /** Duração em segundos de `beats` batidas (semínimas) à taxa `bpm`. */
    inline double beatsToSeconds(double beats, double bpm) noexcept
    {
        if (!fractal_dsp::NumericGuards::isValidPositiveFinite(bpm) || !std::isfinite(beats))
            return 0.0;
        return beats * (60.0 / bpm);
    }

    /** Converte segundos em número inteiro de amostras (arredondado). */
    inline int secondsToSamples(double seconds, double sampleRate) noexcept
    {
        if (!fractal_dsp::NumericGuards::isValidSampleRateAndFiniteSeconds(sampleRate, seconds))
            return 0;
        return static_cast<int>(std::llround(seconds * sampleRate));
    }

    /**
     * Batidas (unidade = semínima) para cada índice do `AudioParameterChoice` delayDivision.
     * Índices 0–7: retas; 8–12: ponteados; 13–17: tríades.
     */
    inline double beatsForDivisionIndex(int index) noexcept
    {
        static constexpr double table[] = {
            4.0,
            2.0,
            1.0,
            0.5,
            0.25,
            0.125,
            0.0625,
            0.03125,
            3.0,
            1.5,
            0.75,
            0.375,
            0.1875,
            4.0 / 3.0,
            2.0 / 3.0,
            1.0 / 3.0,
            1.0 / 6.0,
            1.0 / 12.0
        };
        if (index < 0 || index >= static_cast<int>(std::size(table)))
            return 1.0;
        return table[static_cast<size_t>(index)];
    }

    inline int delayMsToSamples(float delayMs, double sampleRate, int maxDelaySamples) noexcept
    {
        if (!fractal_dsp::NumericGuards::isValidPositiveFinite(sampleRate)
            || !std::isfinite(static_cast<double>(delayMs)))
            return 0;
        const auto s = static_cast<int>(
            std::llround(static_cast<double>(delayMs) * sampleRate / 1000.0));
        return std::clamp(s, 0, maxDelaySamples);
    }

    /**
     * @param syncModeIndex 0 = ms; 1 = sync ao BPM do host.
     * @param hostBpm       BPM válido do host; se vazio em modo sync → fallback ms com `delayMs`.
     */
    inline int resolveDelaySamples(int syncModeIndex,
                                   float delayMs,
                                   int divisionIndex,
                                   const std::optional<double>& hostBpm,
                                   double sampleRate,
                                   int maxDelaySamples) noexcept
    {
        const int msPath = delayMsToSamples(delayMs, sampleRate, maxDelaySamples);
        if (syncModeIndex != 1)
            return msPath;
        if (!hostBpm.has_value())
            return msPath;

        const double beats = beatsForDivisionIndex(divisionIndex);
        const double sec     = beatsToSeconds(beats, *hostBpm);
        if (!fractal_dsp::NumericGuards::isValidPositiveFinite(sec))
            return msPath;

        const int fromSync = secondsToSamples(sec, sampleRate);
        return std::clamp(fromSync, 0, maxDelaySamples);
    }

} // namespace fractal_tempo
