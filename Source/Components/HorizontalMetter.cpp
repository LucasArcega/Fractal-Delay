#include "HorizontalMetter.h"

namespace GUI
{
    void HorizontalMetter::paint(juce::Graphics& g)
    {
        auto bounds = getLocalBounds().toFloat();
        const float rad = juce::jmin(3.f, bounds.getHeight() * 0.45f);

        // Background: tom mais escuro coerente com o tema
        g.setColour(juce::Colour(0xff1a1d23));
        g.fillRoundedRectangle(bounds, rad);

        // Borda sutil
        g.setColour(juce::Colour(0xff3d4450));
        g.drawRoundedRectangle(bounds, rad, 1.f);

        // Barra de nível com gradiente baseado no valor
        const float scaledW = juce::jmap(level, -60.f, 6.f, 0.f, (float) getWidth());
        if (scaledW > 0.f)
        {
            auto filled = bounds.reduced(1.f).withWidth(scaledW);

            // Cor baseada no nível: verde (-60 a -18), amarelo (-18 a -6), vermelho (-6 a +6)
            juce::Colour barColour;
            if (level < -18.f)
                barColour = juce::Colour(0xff4caf50); // Verde
            else if (level < -6.f)
            {
                // Gradiente verde → amarelo
                float t = juce::jmap(level, -18.f, -6.f, 0.f, 1.f);
                barColour = juce::Colour(0xff4caf50).interpolatedWith(juce::Colour(0xffffc107), t);
            }
            else if (level < 0.f)
            {
                // Gradiente amarelo → laranja
                float t = juce::jmap(level, -6.f, 0.f, 0.f, 1.f);
                barColour = juce::Colour(0xffffc107).interpolatedWith(juce::Colour(0xffff9800), t);
            }
            else
            {
                // Gradiente laranja → vermelho
                float t = juce::jmap(level, 0.f, 6.f, 0.f, 1.f);
                barColour = juce::Colour(0xffff9800).interpolatedWith(juce::Colour(0xfff44336), t);
            }

            g.setColour(barColour);
            g.fillRoundedRectangle(filled, rad - 0.5f);
        }
    }

    void HorizontalMetter::setLevel(float levelDb) noexcept
    {
        level = levelDb;
        repaint();
    }
} // namespace GUI
