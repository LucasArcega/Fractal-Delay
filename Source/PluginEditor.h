#pragma once

#include <JuceHeader.h>

#include "Components/FooterBar.h"
#include "PluginProcessor.h"

class FractalDelayAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit FractalDelayAudioProcessorEditor(FractalDelayAudioProcessor&);
    ~FractalDelayAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void idle();

    // Padrão Airwindows: timer separado chama idle() para drenar a fila sem bloquear a GUI.
    struct IdleTimer : juce::Timer
    {
        explicit IdleTimer(FractalDelayAudioProcessorEditor* e) : ed(e) {}
        void timerCallback() override { ed->idle(); }
        FractalDelayAudioProcessorEditor* ed;
    };

private:
    FractalDelayAudioProcessor& audioProcessor;

    std::unique_ptr<IdleTimer> idleTimer;

    // --- Grade macro: cabeçalho | três colunas | rodapé ---
    juce::Label headerLabel;
    GUI::FooterBar footerBar;

    // Colunas (contêineres para o FlexBox interno de cada faixa).
    juce::Component inColumn;
    juce::Component centerColumn;
    juce::Component outColumn;

    juce::Label inTitle;
    juce::Label inLabel;
    juce::Slider inputSlider;

    juce::Label delayTitle;
    juce::Label delayHint;

    juce::Label outTitle;
    juce::Label outLabel;
    juce::Slider outputSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;

    float currentPeakIn        = 0.f;
    float currentPeakOut       = 0.f; // pico máximo na saída (rótulos IN/OUT agregados)
    float currentPeakOutLeft   = 0.f;
    float currentPeakOutRight  = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FractalDelayAudioProcessorEditor)
};
