/*
  Testes Catch2 (executável FractalDelay_Tests).

  Tags: [processor] — só DSP/estado; [gui] — precisa da GUI do JUCE (ver mensagens de SKIP /
  ambiente sem vídeo no caso).
*/

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <cstdlib>
#include <limits>

#include "PluginProcessor.h"
#include "DSP/CircularDelayBuffer.h"
#include "DSP/DelayTempoMath.h"

// --- Smoke: instância e parâmetros registrados ---
TEST_CASE("FractalDelayAudioProcessor instantiates", "[processor]")
{
    FractalDelayAudioProcessor processor;

    REQUIRE_FALSE(processor.getName().isEmpty());
    REQUIRE(processor.getAPVTS().getParameter("inputGainDb") != nullptr);
    REQUIRE(processor.getAPVTS().getParameter("outputGainDb") != nullptr);
    REQUIRE(processor.getAPVTS().getParameter("delayMs") != nullptr);
    REQUIRE(processor.getAPVTS().getParameter("delaySyncMode") != nullptr);
    REQUIRE(processor.getAPVTS().getParameter("delayDivision") != nullptr);
}

// --- Layout de canais suportado pelo plugin ---
TEST_CASE("Stereo bus layout is supported", "[processor]")
{
    FractalDelayAudioProcessor processor;

    juce::AudioProcessor::BusesLayout stereo;
    stereo.inputBuses.add(juce::AudioChannelSet::stereo());
    stereo.outputBuses.add(juce::AudioChannelSet::stereo());

    REQUIRE(processor.isBusesLayoutSupported(stereo));
}

TEST_CASE("Layout de quatro canais arbitrario e rejeitado", "[processor]")
{
    FractalDelayAudioProcessor processor;

    juce::AudioProcessor::BusesLayout quad;
    quad.inputBuses.add(juce::AudioChannelSet::quadraphonic());
    quad.outputBuses.add(juce::AudioChannelSet::quadraphonic());

    REQUIRE_FALSE(processor.isBusesLayoutSupported(quad));
}

// --- Ciclo prepare/release não deve lançar exceção ---
TEST_CASE("prepareToPlay and releaseResources", "[processor]")
{
    FractalDelayAudioProcessor processor;

    REQUIRE_NOTHROW(processor.prepareToPlay(44100.0, 512));
    REQUIRE_NOTHROW(processor.prepareToPlay(48000.0, 64));
    REQUIRE_NOTHROW(processor.releaseResources());
}

// --- Editor: precisa de vídeo; pode ser omitido no ctest ou com variável de ambiente SKIP ---
TEST_CASE("Editor can be created and destroyed", "[processor][gui]")
{
    juce::ScopedJuceInitialiser_GUI juceGui;

    if (std::getenv("SKIP_FRACTAL_DELAY_GUI_TESTS") != nullptr)
        SKIP("SKIP_FRACTAL_DELAY_GUI_TESTS definido (ex.: agente Cursor / shell sem GUI)");

    if (juce::Desktop::getInstance().isHeadless())
        SKIP("Sem display; teste do editor de GUI ignorado");

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
                break; // mensagem de encerramento recebida
        }
    }
#endif

    editor.reset();
    processor.releaseResources();
}

// --- CircularDelayBuffer (fase 1, doc. 02) ---
TEST_CASE("DelayReadbackInteger", "[processor]")
{
    CircularDelayBuffer buf;
    buf.prepare(1, 64, 8);
    buf.reset();

    constexpr int D = 10;

    for (int i = 0; i < 50; ++i)
    {
        (void)buf.readDelayed(0, D);
        buf.pushSample(0, 0.f);
    }

    (void)buf.readDelayed(0, D);
    buf.pushSample(0, 1.f);

    for (int i = 0; i < 9; ++i)
    {
        (void)buf.readDelayed(0, D);
        buf.pushSample(0, 0.f);
    }

    const float v = buf.readDelayed(0, D);
    REQUIRE(std::abs(v - 1.f) < 1e-4f);
}

TEST_CASE("StereoIsolation", "[processor]")
{
    CircularDelayBuffer buf;
    buf.prepare(2, 64, 8);
    buf.reset();

    constexpr int D = 8;

    for (int step = 0; step < 300; ++step)
    {
        (void)buf.readDelayed(0, D);
        (void)buf.readDelayed(1, D);

        const float l = (step == 150) ? 1.f : 0.f;
        buf.pushSample(0, l);
        buf.pushSample(1, 0.f);
    }

    const float rTap = buf.readDelayed(1, D);
    REQUIRE(std::abs(rTap) < 1e-6f);
}

TEST_CASE("PrepareBounds", "[processor]")
{
    CircularDelayBuffer buf;
    buf.prepare(1, 32, 4);
    buf.reset();

    REQUIRE_NOTHROW((void)buf.readDelayed(0, 1000000000));
}

TEST_CASE("ResetClearsState", "[processor]")
{
    CircularDelayBuffer buf;
    buf.prepare(1, 48, 8);
    buf.reset();

    for (int i = 0; i < 80; ++i)
    {
        (void)buf.readDelayed(0, 5);
        buf.pushSample(0, 0.7f);
    }

    buf.reset();

    const float v = buf.readDelayed(0, 5);
    REQUIRE(std::abs(v) < 1e-6f);
}

// --- Delay tempo / BPM (Plans fase 1 doc. 03) ---
TEST_CASE("NumericGuardsPositiveFinite", "[processor]")
{
    using fractal_dsp::NumericGuards;
    REQUIRE_FALSE(NumericGuards::isValidPositiveFinite(0.0));
    REQUIRE_FALSE(NumericGuards::isValidPositiveFinite(-120.0));
    REQUIRE_FALSE(NumericGuards::isValidSampleRateAndFiniteSeconds(44100.0, std::numeric_limits<double>::quiet_NaN()));
    REQUIRE(NumericGuards::isValidPositiveFinite(120.0));
    REQUIRE(NumericGuards::isValidSampleRateAndFiniteSeconds(44100.0, 0.5));
}

TEST_CASE("BeatsToSeconds", "[processor]")
{
    REQUIRE(std::abs(fractal_tempo::beatsToSeconds(1.0, 60.0) - 1.0) < 1e-9);
    REQUIRE(std::abs(fractal_tempo::beatsToSeconds(0.5, 120.0) - 0.25) < 1e-9);
}

TEST_CASE("SubdivisionTable", "[processor]")
{
    REQUIRE(std::abs(fractal_tempo::beatsForDivisionIndex(2) - 1.0) < 1e-12);   // 1/4
    REQUIRE(std::abs(fractal_tempo::beatsForDivisionIndex(3) - 0.5) < 1e-12);   // 1/8
    REQUIRE(std::abs(fractal_tempo::beatsForDivisionIndex(10) - 0.75) < 1e-12); // 1/8 ponteado
    REQUIRE(std::abs(fractal_tempo::beatsForDivisionIndex(15) - (1.0 / 3.0)) < 1e-12); // 1/8 tríade
}

TEST_CASE("InvalidBpmFallback", "[processor]")
{
    REQUIRE(std::abs(fractal_tempo::beatsToSeconds(1.0, 0.0)) < 1e-12);
    REQUIRE(!std::isnan(fractal_tempo::beatsToSeconds(1.0, 0.0)));
    REQUIRE(!std::isnan(fractal_tempo::beatsToSeconds(1.0, -40.0)));

    constexpr int maxS = 100000;
    const int msOnly = fractal_tempo::resolveDelaySamples(0, 100.f, 0, std::nullopt, 44100.0, maxS);
    const int syncNoBpm = fractal_tempo::resolveDelaySamples(1, 100.f, 2, std::nullopt, 44100.0, maxS);
    REQUIRE(msOnly == syncNoBpm);
}

TEST_CASE("PlayHeadAbsent", "[processor]")
{
    constexpr int maxS = 88200;
    const int a = fractal_tempo::resolveDelaySamples(1, 250.f, 2, std::nullopt, 44100.0, maxS);
    const int b = fractal_tempo::resolveDelaySamples(0, 250.f, 2, std::nullopt, 44100.0, maxS);
    REQUIRE(a == b);
}

TEST_CASE("SyncQuarterNoteAt120Bpm", "[processor]")
{
    constexpr int maxS = 200000;
    const std::optional<double> bpm{ 120.0 };
    const int samples = fractal_tempo::resolveDelaySamples(1, 999.f, 2, bpm, 44100.0, maxS);
    // 1 beat @ 120 BPM = 0,5 s → 22050 amostras a 44,1 kHz
    REQUIRE(std::abs(samples - 22050) < 2);
}

