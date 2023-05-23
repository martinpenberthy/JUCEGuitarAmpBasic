/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GuitarAmpBasicAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GuitarAmpBasicAudioProcessorEditor (GuitarAmpBasicAudioProcessor&);
    ~GuitarAmpBasicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton loadButton;
    juce::Label irName;
    
    std::unique_ptr<juce::FileChooser> fileChooser;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuitarAmpBasicAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarAmpBasicAudioProcessorEditor)
};
