/*
  Testes Catch2 (executável FractalDelay_Tests).

  Tags: [processor] — só DSP/estado; [gui] — precisa de JUCE GUI (ver SKIP / headless no caso).
*/

#include <catch2/catch_test_macros.hpp>

#include <cstdlib>

#include "PluginProcessor.h"

// --- Smoke: instância e parâmetros registados ---
TEST_CASE("FractalDelayAudioProcessor instantiates", "[processor]")
{
    FractalDelayAudioProcessor processor;

    REQUIRE_FALSE(processor.getName().isEmpty());
    REQUIRE(processor.getAPVTS().getParameter("inputGainDb") != nullptr);
    REQUIRE(processor.getAPVTS().getParameter("outputGainDb") != nullptr);
}

// --- Layout de canais aceite pelo plugin ---
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

// --- Ciclo prepare/release não deve lançar ---
TEST_CASE("prepareToPlay and releaseResources", "[processor]")
{
    FractalDelayAudioProcessor processor;

    REQUIRE_NOTHROW(processor.prepareToPlay(44100.0, 512));
    REQUIRE_NOTHROW(processor.prepareToPlay(48000.0, 64));
    REQUIRE_NOTHROW(processor.releaseResources());
}

// --- Editor: requer display; pode ser omitido em ctest ou com env SKIP ---
TEST_CASE("Editor can be created and destroyed", "[processor][gui]")
{
    juce::ScopedJuceInitialiser_GUI juceGui;

    if (std::getenv("SKIP_FRACTAL_DELAY_GUI_TESTS") != nullptr)
        SKIP("SKIP_FRACTAL_DELAY_GUI_TESTS set (e.g. Cursor agent / non-GUI shell)");

    if (juce::Desktop::getInstance().isHeadless())
        SKIP("No display; GUI editor test skipped");

    FractalDelayAudioProcessor processor;
    processor.prepareToPlay(44100.0, 512);

    std::unique_ptr<juce::AudioProcessorEditor> editor(processor.createEditor());
    REQUIRE(editor != nullptr);
    REQUIRE(editor->getWidth() > 0);
    REQUIRE(editor->getHeight() > 0);

#if JUCE_MODAL_LOOPS_PERMITTED
    // Drena toda a InternalMessageQueue antes de destruir o editor.
    // Sem este pump, qualquer mensagem JUCE pendente (timers, repaints, APVTS async)
    // acumula na fila interna do Windows sem ser processada e o processo consume GBs de RAM.
    if (auto* mm = juce::MessageManager::getInstanceWithoutCreating())
    {
        const auto deadline = juce::Time::getMillisecondCounter() + 2000u;
        while (juce::Time::getMillisecondCounter() < deadline)
        {
            if (! mm->runDispatchLoopUntil(50))
                break; // quit message recebido
        }
    }
#endif

    editor.reset();
    processor.releaseResources();
}
