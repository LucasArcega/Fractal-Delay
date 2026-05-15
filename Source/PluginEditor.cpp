#include "PluginEditor.h"

FractalDelayAudioProcessorEditor::FractalDelayAudioProcessorEditor(FractalDelayAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
{
    // Define tamanho da janela
    setSize(460, 320);

    // Define estilo do slider de entrada
    inputSlider.setSliderStyle(juce::Slider::LinearVertical);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 22);

    // Track não preenchido (fundo): cinza escuro
    inputSlider.setColour(juce::Slider::backgroundColourId,    juce::Colour(0xff333333));
    // Track preenchido (de baixo até ao thumb): branco
    inputSlider.setColour(juce::Slider::trackColourId,         juce::Colour(0xffffffff));
    
    // Thumb: cinza claro
    inputSlider.setColour(juce::Slider::thumbColourId,         juce::Colour(0xffaaaaaa));
    
    // Caixa de texto abaixo do slider
    inputSlider.setColour(juce::Slider::textBoxTextColourId,       juce::Colours::white);
    inputSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff1a1a1a));
    inputSlider.setColour(juce::Slider::textBoxOutlineColourId,    juce::Colours::transparentBlack);
    addAndMakeVisible(inputSlider);

    // Define estilo do slider de saída

    outputSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0xff333333));
    outputSlider.setColour(juce::Slider::trackColourId,         juce::Colour(0xffffffff));
    outputSlider.setColour(juce::Slider::thumbColourId,         juce::Colour(0xffaaaaaa));
    outputSlider.setColour(juce::Slider::textBoxTextColourId,       juce::Colours::white);
    outputSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff1a1a1a));
    outputSlider.setColour(juce::Slider::textBoxOutlineColourId,    juce::Colours::transparentBlack);
    outputSlider.setSliderStyle(juce::Slider::LinearVertical);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 22);
    addAndMakeVisible(outputSlider);

    // Define attachment dos sliders
    inputAttachment  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "inputGainDb",  inputSlider);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "outputGainDb", outputSlider);

    // Define estilo do label de entrada
    inLabel.setText("IN: ---", juce::dontSendNotification);
    inLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(inLabel);

    // Define estilo do label de saída
    outLabel.setText("OUT: ---", juce::dontSendNotification);
    outLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(outLabel);

    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000 / 30);
}

FractalDelayAudioProcessorEditor::~FractalDelayAudioProcessorEditor()
{
    idleTimer->stopTimer();
}

void FractalDelayAudioProcessorEditor::idle()
{
    FractalDelayAudioProcessor::AudioToUIMessage msg;
    bool needsRepaint = false;

    while (audioProcessor.audioToUI.pop(msg))
    {
        switch (msg.what)
        {
            case FractalDelayAudioProcessor::AudioToUIMessage::PEAK_IN:
                currentPeakIn = msg.newValue;
                break;
            case FractalDelayAudioProcessor::AudioToUIMessage::PEAK_OUT:
                currentPeakOut = msg.newValue;
                break;
            case FractalDelayAudioProcessor::AudioToUIMessage::INCREMENT:
                needsRepaint = true;
                break;
        }
    }

    if (needsRepaint)
    {
        auto toDb = [](float linear) -> juce::String
        {
            if (linear < 1e-6f) return "-inf dB";
            return juce::String(juce::Decibels::gainToDecibels(linear), 1) + " dB";
        };

        inLabel.setText("IN: "  + toDb(currentPeakIn),  juce::dontSendNotification);
        outLabel.setText("OUT: " + toDb(currentPeakOut), juce::dontSendNotification);
    }
}

void FractalDelayAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1d23));
}

void FractalDelayAudioProcessorEditor::resized()
{
    // Altura fixa para os sliders — não crescem com a janela
    constexpr int labelH  = 24;
    constexpr int sliderH = 200;

    auto area = getLocalBounds().reduced(20);

    auto left  = area.removeFromLeft(area.getWidth() / 2).reduced(10, 0);
    auto right = area.reduced(10, 0);

    inLabel.setBounds(left.removeFromTop(labelH));
    inputSlider.setBounds(left.removeFromTop(sliderH));

    outLabel.setBounds(right.removeFromTop(labelH));
    outputSlider.setBounds(right.removeFromTop(sliderH));
}
