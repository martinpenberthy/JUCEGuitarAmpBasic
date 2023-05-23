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
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    variableTree = {
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
}

GuitarAmpBasicAudioProcessor::~GuitarAmpBasicAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout GuitarAmpBasicAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
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

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
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

    juce::dsp::AudioBlock<float> block (buffer);
    
    if(irLoader.getCurrentIRSize() > 0)
        irLoader.process(juce::dsp::ProcessContextReplacing<float>(block));
    
}

void GuitarAmpBasicAudioProcessor::reset()
{
    irLoader.reset(); // [3]
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
        
        
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GuitarAmpBasicAudioProcessor();
}
