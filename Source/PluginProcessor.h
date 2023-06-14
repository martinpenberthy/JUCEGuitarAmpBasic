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
class GuitarAmpBasicAudioProcessor  : public juce::AudioProcessor, public juce::ValueTree::Listener
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
    void updateFilter();
    
    
    //Level meter functions
    float getRMSValueInput(const int channel) const;
    float getRMSValueOutput(const int channel) const;

    juce::AudioProcessorValueTreeState treeState;
    juce::ValueTree variableTree;
    
    std::string waveshapeFunction;
    std::string waveshapeFunctionCurrent;
    
    bool isInput;
    /*float inputGainVal {0.0f};
    
    float preEQVal {5.0f};
    float preGainVal1 {0.0f};
    float preGainVal2 {0.0f};
    float preGainVal3 {0.0f};
    
    float postGainVal {0.0f};*/ 

    //juce::dsp::LadderFilterMode filterMode;
    
    juce::File root, savedFile;
    juce::dsp::Convolution irLoader;
private:
    enum
    {
        //inputGainIndex,
        preEQIndex,
        lowEQIndex,
        preGainIndex1,
        waveshaperIndex1,
        preGainIndex2,
        waveshaperIndex2,
        preGainIndex3,
        waveshaperIndex3,
        filterLowIndex,
        filterMidIndex,
        filterHighIndex//,
        //postGainIndex
    };
    
    using IIRFilter = juce::dsp::IIR::Filter<float>;
    using IIRCoefs = juce::dsp::IIR::Coefficients<float>;
    
    juce::dsp::Gain<float> inputGain;
    
    juce::dsp::ProcessorChain<//juce::dsp::Gain<float>,
                              juce::dsp::LadderFilter<float>, //PreEQ
                              juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>, //LowEQ
                              juce::dsp::Gain<float>, //Pregain1
                              juce::dsp::WaveShaper<float>, //Waveshaper1
                              juce::dsp::Gain<float>, //Pregain2
                              juce::dsp::WaveShaper<float>, //Waveshaper2
                              juce::dsp::Gain<float>, //Pregain3
                              juce::dsp::WaveShaper<float>, //Waveshaper3
                              juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>,
                              juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>,
                              juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>
                              /*,juce::dsp::Gain<float>*/> processorChain;
    
    juce::dsp::Gain<float> outputGain;
    
    
    //juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filterHigh;
    juce::dsp::ProcessSpec spec;
    juce::LinearSmoothedValue<float> rmsLevelInput;
    juce::LinearSmoothedValue<float> rmsLevelOutput;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarAmpBasicAudioProcessor)
};
