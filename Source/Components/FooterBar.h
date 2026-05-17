#pragma once

#include <JuceHeader.h>

#include "HorizontalMetter.h"

namespace GUI
{
/** Rodapé em “caixa”: dois medidores da saída estéreo (L em cima, R em baixo). */
class FooterBar : public juce::Component
{
public:
    FooterBar();

    void paint(juce::Graphics& g) override;
    void resized() override;

    HorizontalMetter& getOutputMeterLeft() noexcept { return outputMeterLeft; }
    HorizontalMetter& getOutputMeterRight() noexcept { return outputMeterRight; }

private:
    HorizontalMetter outputMeterLeft;
    HorizontalMetter outputMeterRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterBar)
};
} // namespace GUI
