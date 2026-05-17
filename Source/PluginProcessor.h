#pragma once
#include <JuceHeader.h>

class FractalDelayAudioProcessor : public juce::AudioProcessor
{
public:
    FractalDelayAudioProcessor();
    ~FractalDelayAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() noexcept { return apvts; }
    const juce::AudioProcessorValueTreeState& getAPVTS() const noexcept { return apvts; }

    // Mensagens da thread de áudio para a UI — padrão Airwindows Meter
    struct AudioToUIMessage
    {
        enum What
        {
            PEAK_IN,
            PEAK_OUT,       // pico máximo na saída (todos os canais) — labels IN/OUT
            PEAK_OUT_LEFT,  // saída canal esquerdo
            PEAK_OUT_RIGHT, // saída canal direito
            INCREMENT
        } what { INCREMENT };
        float newValue = 0.f;
    };

    template <typename T, int qSize = 512>
    class LockFreeQueue
    {
    public:
        LockFreeQueue() : fifo(qSize) {}

        bool push(const T& item)
        {
            int start1, size1, start2, size2;
            fifo.prepareToWrite(1, start1, size1, start2, size2);
            if (size1 < 1) return false;
            data[start1] = item;
            fifo.finishedWrite(1);
            return true;
        }

        bool pop(T& item)
        {
            int start1, size1, start2, size2;
            fifo.prepareToRead(1, start1, size1, start2, size2);
            if (size1 < 1) return false;
            item = data[start1];
            fifo.finishedRead(1);
            return true;
        }

    private:
        juce::AbstractFifo fifo;
        T data[qSize];
    };

    LockFreeQueue<AudioToUIMessage> audioToUI;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioProcessorValueTreeState apvts;

    double currentSampleRate = 44100.0;
    int rmsCount = 0;
    int rmsSize  = 1882;   // ~43 ms a 44100 Hz — igual ao Airwindows
    float peakIn = 0.f;
    float peakOut = 0.f;
    float peakOutLeft = 0.f;
    float peakOutRight = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FractalDelayAudioProcessor)
};
