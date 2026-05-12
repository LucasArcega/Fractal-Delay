#pragma once

#include <JuceHeader.h>

class FractalDelayAudioProcessor;

class FractalDelayAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit FractalDelayAudioProcessorEditor(FractalDelayAudioProcessor&);
    void resized() override;

private:
    FractalDelayAudioProcessor& audioProcessor;
    std::unique_ptr<juce::GenericAudioProcessorEditor> genericEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FractalDelayAudioProcessorEditor)
};
