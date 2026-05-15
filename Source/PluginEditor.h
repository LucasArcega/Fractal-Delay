#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class FractalDelayAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit FractalDelayAudioProcessorEditor(FractalDelayAudioProcessor&);
    ~FractalDelayAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void idle();

    // Padrão Airwindows: struct de timer separado que chama idle()
    struct IdleTimer : juce::Timer
    {
        explicit IdleTimer(FractalDelayAudioProcessorEditor* e) : ed(e) {}
        void timerCallback() override { ed->idle(); }
        FractalDelayAudioProcessorEditor* ed;
    };

// Variáveis de instância
private:
    FractalDelayAudioProcessor& audioProcessor;

    std::unique_ptr<IdleTimer> idleTimer;

    juce::Label inLabel;
    juce::Label outLabel;

    juce::Slider inputSlider;
    juce::Slider outputSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;

    float currentPeakIn  = 0.f;
    float currentPeakOut = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FractalDelayAudioProcessorEditor)
};
