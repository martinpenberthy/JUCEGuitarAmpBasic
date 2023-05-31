    /*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class GuitarAmpBasicAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    GuitarAmpBasicAudioProcessor();
    ~GuitarAmpBasicAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void setFunctionToUse(std::string func);
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void reset() override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::AudioProcessorValueTreeState treeState;
    juce::ValueTree variableTree;
    
    std::string waveshapeFunction;
    std::string waveshapeFunctionCurrent;
    
    float preEQVal {5.0f};
    float preGainVal1 {0.0f};
    float preGainVal2 {0.0f};
    
    float postGainVal {0.0f};

    juce::dsp::LadderFilterMode filterMode;

    

    
    juce::File root, savedFile;
    juce::dsp::Convolution irLoader;
private:
    enum
    {
        preEQIndex,
        preGainIndex1,
        waveshaperIndex1,
        lowEQIndex,
        preGainIndex2,
        waveshaperIndex2,
        postGainIndex
    };
    
    juce::dsp::ProcessorChain<juce::dsp::LadderFilter<float>, juce::dsp::Gain<float>, juce::dsp::WaveShaper<float>, juce::dsp::LadderFilter<float>, juce::dsp::Gain<float>, juce::dsp::WaveShaper<float>, juce::dsp::Gain<float>> processorChain;
    juce::dsp::ProcessSpec spec;
    

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarAmpBasicAudioProcessor)
};
