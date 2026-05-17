#include "PluginEditor.h"

namespace
{
    void applyMonoSliderStyle(juce::Slider& s)
    {
        s.setSliderStyle(juce::Slider::LinearVertical);
        s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 22);
        s.setColour(juce::Slider::backgroundColourId, juce::Colour(0xff333333));
        s.setColour(juce::Slider::trackColourId, juce::Colour(0xffffffff));
        s.setColour(juce::Slider::thumbColourId, juce::Colour(0xffaaaaaa));
        s.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        s.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff1a1a1a));
        s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    void styleSmallTitle(juce::Label& l, const juce::String& text)
    {
        l.setText(text, juce::dontSendNotification);
        l.setJustificationType(juce::Justification::centred);
        l.setFont(juce::Font(juce::FontOptions(14.f)).boldened());
        l.setColour(juce::Label::textColourId, juce::Colours::white);
    }

    void stylePeakLabel(juce::Label& l, const juce::String& initial)
    {
        l.setText(initial, juce::dontSendNotification);
        l.setJustificationType(juce::Justification::centred);
        l.setFont(juce::Font(juce::FontOptions(12.f)));
        l.setColour(juce::Label::textColourId, juce::Colour(0xffaaaaaa));
    }

    /** Coluna IN/OUT: título, medidor, slider (altura flex, mínimo fixo). */
    void layoutSideStrip(juce::Component& column,
                         juce::Label& title,
                         juce::Label& peak,
                         juce::Slider& slider)
    {
        auto r = column.getLocalBounds().toFloat();
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
        fb.alignItems     = juce::FlexBox::AlignItems::stretch;

        constexpr float titleH = 20.f;
        constexpr float peakH  = 22.f;
        constexpr float sliderMinH = 120.f;

        fb.items.add(juce::FlexItem(title).withHeight(titleH));
        fb.items.add(juce::FlexItem(peak).withHeight(peakH));
        fb.items.add(juce::FlexItem(slider).withFlex(1.f).withMinHeight(sliderMinH).withMaxWidth((float) column.getWidth()));

        fb.performLayout(r);
    }

    /** Coluna central: título + dica (placeholder até haver parâmetros de delay). */
    void layoutCenterStrip(juce::Component& column, juce::Label& title, juce::Label& hint)
    {
        auto r = column.getLocalBounds().toFloat();
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignItems     = juce::FlexBox::AlignItems::stretch;

        fb.items.add(juce::FlexItem(title).withHeight(22.f));
        fb.items.add(juce::FlexItem(hint).withFlex(1.f).withMinHeight(40.f));

        fb.performLayout(r);
    }
} // namespace

FractalDelayAudioProcessorEditor::FractalDelayAudioProcessorEditor(FractalDelayAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
{
    // Janela um pouco mais larga para 3 colunas (Grid + FlexBox)
    setSize(640, 360);

    headerLabel.setText("Fractal Delay", juce::dontSendNotification);
    headerLabel.setJustificationType(juce::Justification::centredLeft);
    headerLabel.setFont(juce::Font(juce::FontOptions(18.f)).boldened());
    headerLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(headerLabel);

    addAndMakeVisible(footerBar);

    addAndMakeVisible(inColumn);
    addAndMakeVisible(centerColumn);
    addAndMakeVisible(outColumn);

    styleSmallTitle(inTitle, "IN");
    stylePeakLabel(inLabel, "IN: ---");
    applyMonoSliderStyle(inputSlider);
    inColumn.addAndMakeVisible(inTitle);
    inColumn.addAndMakeVisible(inLabel);
    inColumn.addAndMakeVisible(inputSlider);

    styleSmallTitle(outTitle, "OUT");
    stylePeakLabel(outLabel, "OUT: ---");
    applyMonoSliderStyle(outputSlider);
    outColumn.addAndMakeVisible(outTitle);
    outColumn.addAndMakeVisible(outLabel);
    outColumn.addAndMakeVisible(outputSlider);

    delayTitle.setText("Delay", juce::dontSendNotification);
    delayTitle.setJustificationType(juce::Justification::centred);
    delayTitle.setFont(juce::Font(juce::FontOptions(15.f)).boldened());
    delayTitle.setColour(juce::Label::textColourId, juce::Colours::white);

    delayHint.setText(
        juce::translate("Time, feedback, mix and damping will live here."),
        juce::dontSendNotification);
    delayHint.setJustificationType(juce::Justification::centred);
    delayHint.setFont(juce::Font(juce::FontOptions(12.f)));
    delayHint.setColour(juce::Label::textColourId, juce::Colour(0xff9aa7b8));
    centerColumn.addAndMakeVisible(delayTitle);
    centerColumn.addAndMakeVisible(delayHint);

    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "inputGainDb", inputSlider);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "outputGainDb", outputSlider);

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
            case FractalDelayAudioProcessor::AudioToUIMessage::PEAK_OUT_LEFT:
                currentPeakOutLeft = msg.newValue;
                break;
            case FractalDelayAudioProcessor::AudioToUIMessage::PEAK_OUT_RIGHT:
                currentPeakOutRight = msg.newValue;
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
            if (linear < 1e-6f)
                return "-inf dB";
            return juce::String(juce::Decibels::gainToDecibels(linear), 1) + " dB";
        };

        inLabel.setText("IN: " + toDb(currentPeakIn), juce::dontSendNotification);
        outLabel.setText("OUT: " + toDb(currentPeakOut), juce::dontSendNotification);

        auto linearToMeterDb = [](float lin)
        {
            if (lin < 1e-8f)
                return -60.f;
            return juce::jlimit(-60.f, 6.f, juce::Decibels::gainToDecibels(lin));
        };
        footerBar.getOutputMeterLeft().setLevel(linearToMeterDb(currentPeakOutLeft));
        footerBar.getOutputMeterRight().setLevel(linearToMeterDb(currentPeakOutRight));
    }
}

void FractalDelayAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1d23));

    // Linha separadora acima do footer (vai de ponta a ponta)
    const int footerY = footerBar.getY();
    g.setColour(juce::Colour(0xff3d4450));
    g.drawHorizontalLine(footerY, 0.f, (float) getWidth());
}

void FractalDelayAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    // Footer ocupa toda a largura (fora do grid)
    const int footerHeight = 52;
    auto footerArea = area.removeFromBottom(footerHeight);
    footerBar.setBounds(footerArea);

    // Área restante com padding para o grid
    area.removeFromBottom(8); // Gap antes do footer
    const auto bounds = area.reduced(14, 14);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr    = juce::Grid::Fr;
    using Px    = juce::Grid::Px;

    // Sem o footer, só header e corpo
    grid.templateRows = { Track(Px(28)), Track(Fr(1)) };
    // Centro um pouco mais largo (100 : 145 : 100 ≈ 1 : 1.45 : 1)
    grid.templateColumns = { Track(Fr(100)), Track(Fr(145)), Track(Fr(100)) };
    grid.rowGap    = Px(8);
    grid.columnGap = Px(10);

    grid.items = {
        juce::GridItem(headerLabel).withArea(1, 1, 1, 3),
        juce::GridItem(inColumn).withArea(2, 1),
        juce::GridItem(centerColumn).withArea(2, 2),
        juce::GridItem(outColumn).withArea(2, 3),
    };

    grid.performLayout(bounds);

    layoutSideStrip(inColumn, inTitle, inLabel, inputSlider);
    layoutCenterStrip(centerColumn, delayTitle, delayHint);
    layoutSideStrip(outColumn, outTitle, outLabel, outputSlider);
}
