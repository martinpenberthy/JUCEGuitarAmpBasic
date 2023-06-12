/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarAmpBasicAudioProcessor::GuitarAmpBasicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())//, filterHigh(juce::dsp::IIR::Coefficients<float>::makePeakFilter(44100, 9000.0f, 0.1f, 1.0f))
#endif
{
    variableTree =
    {
        "Variables", {},
        {
            { "Group", {{"name", "IR Vars"}},
                {
                    {"Parameter", {{"id", "file1"}, {"value", "/"}}},
                    {"Parameter", {{"id", "root"}, {"value", "/"}}}
                }
            }
        }
    };

    //treeState.state.addListener(*this);
}

GuitarAmpBasicAudioProcessor::~GuitarAmpBasicAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout GuitarAmpBasicAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUTGAIN", "InputGain", -96.0f, 48.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PREGAIN1", "PreGain1", 0.0f, 48.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PREGAIN2", "PreGain2", 0.0f, 48.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PREGAIN3", "PreGain3", 0.0f, 48.0f, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POSTGAIN", "PostGain", -96.0f, 48.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("PREEQ", "PreEQ", 1.0f, 10.0f, 5.0f));
                    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIGH", "High", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID", "Mid", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW", "Low", 0.0f, 2.0f, 1.0f));


    params.push_back(std::make_unique<juce::AudioParameterChoice>("TYPE1", "Type1",
                                                            juce::StringArray {"Tanh", "AmpTest", "x/abs(x)+1", "Atan", "HalfRect",
                                                                                "Amp1"},
                                                            1));
    return {params.begin(), params.end()};
}

void GuitarAmpBasicAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    
}


//==============================================================================
const juce::String GuitarAmpBasicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GuitarAmpBasicAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GuitarAmpBasicAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GuitarAmpBasicAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GuitarAmpBasicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GuitarAmpBasicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GuitarAmpBasicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GuitarAmpBasicAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GuitarAmpBasicAudioProcessor::getProgramName (int index)
{
    return {};
}

void GuitarAmpBasicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GuitarAmpBasicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    rmsLevel.reset(sampleRate, 0.5f);

    
    rmsLevel.setCurrentAndTargetValue(-100.0f);
    
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    reset();
    
    /*juce::Value restoreWaveshapeFunction = treeState.getParameterAsValue("TYPE1");
    
    std::string restoreWaveshapeFunctionString = restoreWaveshapeFunction.getValue().toString().toStdString();
    setFunctionToUse(restoreWaveshapeFunctionString);
    waveshapeFunction = restoreWaveshapeFunctionString;*/
    
    //Set up waveshaper
    /*auto &waveshaper1 = processorChain.get<waveshaperIndex1>();
    waveshaper1.functionToUse = [](float x)
    {   
        return x / (std::abs(x) + 1);
    };
    waveshapeFunctionCurrent = "x/abs(x)+1";
    waveshapeFunction = "x/abs(x)+1";*/
    
    auto waveshapeInitFunction = treeState.getRawParameterValue("TYPE1");
    switch((int)*waveshapeInitFunction){
        case 1:
            setFunctionToUse("Tanh");
            waveshapeFunction = "Tanh";
            break;
        case 2:
            setFunctionToUse("AmpTest");
            waveshapeFunction = "AmpTest";
            break;
        case 3:
            setFunctionToUse("x/abs(x)+1");
            waveshapeFunction ="x/abs(x)+1";
            break;
        case 4:
            setFunctionToUse("Atan");
            waveshapeFunction = "Atan";
            break;
        case 5:
            setFunctionToUse("HalfRect");
            waveshapeFunction = "HalfRect";
            break;
        case 6:
            setFunctionToUse("Amp1");
            waveshapeFunction = "Amp1";
            break;
            
        default:
            setFunctionToUse("Tanh");
            waveshapeFunction = "Tanh";
            break;
    }
    

    auto &waveshaper2 = processorChain.get<waveshaperIndex2>();
    waveshaper2.functionToUse = [](float x)
    {
        //return x;
        return x / (std::abs(x) + 1);
        //return (x * x) / 2.0f;
    };
    
    
    auto &waveshaper3 = processorChain.get<waveshaperIndex3>();
    waveshaper3.functionToUse = [](float x)
    {
        //return x;
        return x / (std::abs(x) + 1);
        //return (x * x) / 2.0f;
        //return std::atan(x);
    };
    
    //auto &inputGain = processorChain.get<inputGainIndex>();
    
    inputGain.setGainDecibels(0.0f);
    outputGain.setGainDecibels(0.0f);
    
    
    //Set up pre and post gain
    auto &preGain1 = processorChain.get<preGainIndex1>();
    preGain1.setGainDecibels(0.0f);
    
    auto &preGain2 = processorChain.get<preGainIndex2>();
    preGain2.setGainDecibels(0.0f);
    
    auto &preGain3 = processorChain.get<preGainIndex3>();
    preGain3.setGainDecibels(0.0f);
    
    //auto &postGain = processorChain.get<postGainIndex>();
    //postGain.setGainDecibels(0.0f);
    
    //Set up PreEQ
    auto &preEQ = processorChain.get<preEQIndex>();
    preEQ.setMode(juce::dsp::LadderFilterMode::LPF12);
    preEQ.setResonance(0.2);
    preEQ.setCutoffFrequencyHz(5000.0f);
    preEQ.setDrive(1.0f);
    
    /*auto &lowEQ = processorChain.get<lowEQIndex>();
    lowEQ.setMode(juce::dsp::LadderFilterMode::HPF12);
    lowEQ.setResonance(0.1);
    lowEQ.setCutoffFrequencyHz(300.0f);
    lowEQ.setDrive(1.0f);*/
    auto &lowEQ = processorChain.get<lowEQIndex>();
    *lowEQ.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 400.0f, 0.4f, 0.1f);
    
    
    auto &filterHigh = processorChain.get<filterHighIndex>();
    *filterHigh.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 5000.0f, 0.6f, 1.0f);
    
    auto &filterMid = processorChain.get<filterMidIndex>();
    *filterMid.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 500.0f, 0.9f, 1.0f);
    
    auto &filterLow = processorChain.get<filterLowIndex>();
    *filterLow.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 100.0f, 0.6f, 1.0f);

    
    processorChain.prepare(spec);

    //Prepare convolution
    irLoader.reset();
    irLoader.prepare(spec);
    
    if(savedFile.existsAsFile())
    {
        irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes,juce::dsp::Convolution::Trim::yes, 0);
    }
}



void GuitarAmpBasicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GuitarAmpBasicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void GuitarAmpBasicAudioProcessor::updateFilter()
{
    float gainHigh = *treeState.getRawParameterValue("HIGH");
    float gainMid = *treeState.getRawParameterValue("MID");
    float gainLow = *treeState.getRawParameterValue("LOW");
    
    if(gainHigh == 0.00f)
        gainHigh = 0.01f;
    
    if(gainMid == 0.00f)
        gainMid = 0.01f;
    
    if(gainLow == 0.00f)
        gainLow = 0.01f;
    
    auto &filterHigh = processorChain.get<filterHighIndex>();
    *filterHigh.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 5000.0f, 0.6f, gainHigh);

    auto &filterMid = processorChain.get<filterMidIndex>();
    *filterMid.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 500.0f, 0.9f, gainMid);
    
    auto &filterLow = processorChain.get<filterLowIndex>();
    *filterLow.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 100.0f, 0.6f, gainLow);
    
}

void GuitarAmpBasicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    
    
    if(waveshapeFunction != waveshapeFunctionCurrent)
        setFunctionToUse(waveshapeFunction);
    
    //auto &inputGain = processorChain.get<inputGainIndex>();
    inputGain.setGainDecibels(inputGainVal);
    
    juce::dsp::AudioBlock<float> inputGainBlock (buffer);
    inputGain.process(juce::dsp::ProcessContextReplacing<float>(inputGainBlock));
    
    //Set values for pre gains
    auto &preGain1 = processorChain.get<preGainIndex1>();
    preGain1.setGainDecibels(preGainVal1);
    
    auto &preGain2 = processorChain.get<preGainIndex2>();
    preGain2.setGainDecibels(preGainVal2);
    
    auto &preGain3 = processorChain.get<preGainIndex3>();
    preGain3.setGainDecibels(preGainVal3);
    
    //Set value for post gain
    //auto &postGain = processorChain.get<postGainIndex>();

    
    //Set value for pre EQ
    auto &preEQ = processorChain.get<preEQIndex>();
    preEQ.setCutoffFrequencyHz(preEQVal * 1000.0f);
    
    updateFilter();
    
    
    //Process waveshaper chain
    juce::dsp::AudioBlock<float> processorChainBlock (buffer);
    processorChain.process(juce::dsp::ProcessContextReplacing<float>(processorChainBlock));
    
    //Process convolver
    juce::dsp::AudioBlock<float> convolverBlock (buffer);
    
    if(irLoader.getCurrentIRSize() > 0)
        irLoader.process(juce::dsp::ProcessContextReplacing<float>(convolverBlock));
    
    
    outputGain.setGainDecibels(postGainVal);
    juce::dsp::AudioBlock<float> outputGainBlock (buffer);
    outputGain.process(juce::dsp::ProcessContextReplacing<float>(outputGainBlock));
    
    
    /*Set val for RMS level meter*/
    rmsLevel.skip(buffer.getNumSamples());
    
    const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    if(value < rmsLevel.getCurrentValue())
        rmsLevel.setTargetValue(value);
    else
        rmsLevel.setCurrentAndTargetValue(value);

}

void GuitarAmpBasicAudioProcessor::reset()
{
    processorChain.reset();
    irLoader.reset(); // [3]
    //filterHigh.reset();
}

float GuitarAmpBasicAudioProcessor::getRMSValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if(channel == 0)
        return rmsLevel.getCurrentValue();
    if(channel == 1)
        return rmsLevel.getCurrentValue();
    return 0;
}

//==============================================================================
bool GuitarAmpBasicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GuitarAmpBasicAudioProcessor::createEditor()
{
    return new GuitarAmpBasicAudioProcessorEditor (*this);
}

//==============================================================================
void GuitarAmpBasicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream(stream);
    

}

void GuitarAmpBasicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if(tree.isValid())
    {
        treeState.state = tree;
        
        savedFile = juce::File(variableTree.getProperty("file1"));
        root = juce::File(variableTree.getProperty("root"));
        
        irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }
    

}


void GuitarAmpBasicAudioProcessor::setFunctionToUse(std::string func)
{
    auto &waveshaper1 = processorChain.get<waveshaperIndex1>();
    
    if(func == "Tanh")
    {
        waveshaper1.functionToUse = [](float x)
        {
            return std::tanh (x);
        };
        waveshapeFunctionCurrent = "Tanh";
    }
    else if(func == "x/abs(x)+1")
    {
        waveshaper1.functionToUse = [](float x)
        {
            return x / (std::abs(x) + 1);
        };
        waveshapeFunctionCurrent = "x/abs(x)+1";
    }
    else if(func == "AmpTest")
    {
        waveshaper1.functionToUse = [](float x)
        {
            //float param = 0.9f;
            return (x * (std::abs(x) + 0.9f)) * 1.5f / (x * x + (0.3f) * (0.1f / std::abs(x)) + 1.0f) * 0.6f;

            //return (x * (std::abs(x) + param) / (x * x + (param - 1.0f) * std::abs(x) + 1.0f)) * 0.7f;
            //return ((x / (std::abs(x) + param) * 1.5f ) / (x * x + (0.0f - 1.0f) * std::abs(x) + 1.0f)) * 0.7f;
            //return std::tan(x / 1.0f);
        };
        waveshapeFunctionCurrent = "AmpTest";
    }
    else if(func == "Atan")
    {
        waveshaper1.functionToUse = [](float x)
        {
            return std::atan(x);
        };
        waveshapeFunctionCurrent = "Atan";
    }
    else if(func == "HalfRect")
    {
        waveshaper1.functionToUse = [](float x)
        {
            if(x < 0.0f)
                return 0.0f;
            else
                return x;
        };
        waveshapeFunctionCurrent = "HalfRect";
    }
    else if(func == "Amp1")
    {
        waveshaper1.functionToUse = [](float x)
        {
            float param = 0.9f;
            //return (x * (std::abs(x) + param) / (x * x + (param - 1.0f) * std::abs(x) + 1.0f)) * 0.7f;
            return ((x / (std::abs(x) + param) * 1.5f ) / (x * x + (0.0f - 1.0f) * std::abs(x) + 1.0f)) * 0.7f;
            //x * (std::abs(x) + param) / (x * x + (param - 1.0f) * std::abs(x) + 1.0f) * 0.7f;
        };
        waveshapeFunctionCurrent = "Amp1";
    }
    return;
}




//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GuitarAmpBasicAudioProcessor();
}
