#include <catch2/catch_test_macros.hpp>

#include "PluginProcessor.h"

TEST_CASE("FractalDelayAudioProcessor instantiates", "[processor]")
{
    FractalDelayAudioProcessor processor;

    REQUIRE_FALSE(processor.getName().isEmpty());
}

TEST_CASE("Stereo bus layout is supported", "[processor]")
{
    FractalDelayAudioProcessor processor;

    juce::AudioProcessor::BusesLayout stereo;
    stereo.inputBuses.add(juce::AudioChannelSet::stereo());
    stereo.outputBuses.add(juce::AudioChannelSet::stereo());

    REQUIRE(processor.isBusesLayoutSupported(stereo));
}

TEST_CASE("Arbitrary quad layout is rejected", "[processor]")
{
    FractalDelayAudioProcessor processor;

    juce::AudioProcessor::BusesLayout quad;
    quad.inputBuses.add(juce::AudioChannelSet::quadraphonic());
    quad.outputBuses.add(juce::AudioChannelSet::quadraphonic());

    REQUIRE_FALSE(processor.isBusesLayoutSupported(quad));
}

TEST_CASE("prepareToPlay and releaseResources", "[processor]")
{
    FractalDelayAudioProcessor processor;

    REQUIRE_NOTHROW(processor.prepareToPlay(44100.0, 512));
    REQUIRE_NOTHROW(processor.prepareToPlay(48000.0, 64));
    REQUIRE_NOTHROW(processor.releaseResources());
}

TEST_CASE("Editor can be created and destroyed", "[processor][gui]")
{
    juce::ScopedJuceInitialiser_GUI juceGui;

    FractalDelayAudioProcessor processor;

    std::unique_ptr<juce::AudioProcessorEditor> editor(processor.createEditor());
    REQUIRE(editor != nullptr);
    REQUIRE(editor->getWidth() > 0);
    REQUIRE(editor->getHeight() > 0);
}
