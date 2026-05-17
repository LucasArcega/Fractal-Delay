#pragma once

#include <JuceHeader.h>

namespace GUI
{
    /** Medidor horizontal simples (nível em dB, faixa aprox. -60 … +6). */
    class HorizontalMetter : public juce::Component
    {
        public:
            void paint(juce::Graphics& g) override;

            void setLevel(float levelDb) noexcept;
            [[nodiscard]] float getLevel() const noexcept { return level; }

        private:
            float level = -60.f;
        };
} // namespace GUI
