#pragma once

#include <cmath>

namespace fractal_dsp
{

    /**
     * Validações numéricas reutilizáveis na thread de áudio (evita NaN, infinito e não positivos
     * antes de conversões tempo ↔ amostras, coeficientes, etc.).
     */
    struct NumericGuards
    {
        NumericGuards() = delete;

        /** Valor estritamente > 0 e finito (BPM, taxa de amostragem, duração em segundos > 0, …). */
        static bool isValidPositiveFinite(double x) noexcept
        {
            return (x > 0.0) && std::isfinite(x);
        }

        /** Taxa de amostragem válida e duração em segundos finita (conversão tempo → amostras). */
        static bool isValidSampleRateAndFiniteSeconds(double sampleRate, double seconds) noexcept
        {
            return isValidPositiveFinite(sampleRate) && std::isfinite(seconds);
        }
    };

} // namespace fractal_dsp
