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
    
    void modeMenuChanged();

private:
    juce::TextButton loadButton;
    juce::Label irName;
    
    std::unique_ptr<juce::FileChooser> fileChooser;
    
    juce::Label labelWaveshapeType1;//Label for menu
    juce::ComboBox waveshapeType1; //Menu selecting waveshaping function
    
    juce::Slider sliderPreGain1;//Pregain slider
    juce::Label labelPreGain1; //Pregain label
    
    juce::Slider sliderPreGain2;//Pregain slider
    juce::Label labelPreGain2; //Pregain label
    
    juce::Slider sliderPostGain;//Postgain slider
    juce::Label labelPostGain; //Postgain label
    
    juce::Slider sliderPreEQ;//PreEQ slider
    juce::Label labelPreEQ; //PreEQ label

    //juce::Slider sliderFilterHighGain;
    //juce::Label labelFilterHighGain;
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPreGain1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPreGain2;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPostGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPreEQ;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachmentWaveshapeType1;

    //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentFilterHighGain;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuitarAmpBasicAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarAmpBasicAudioProcessorEditor)
};
