#include "PluginEditor.h"
#include "PluginProcessor.h"

FractalDelayAudioProcessorEditor::FractalDelayAudioProcessorEditor(FractalDelayAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
    , genericEditor(std::make_unique<juce::GenericAudioProcessorEditor>(p))
{
    addAndMakeVisible(*genericEditor);
    setSize(genericEditor->getWidth(), juce::jmax(240, genericEditor->getHeight()));
}

void FractalDelayAudioProcessorEditor::resized()
{
    if (genericEditor != nullptr)
        genericEditor->setBounds(getLocalBounds());
}
