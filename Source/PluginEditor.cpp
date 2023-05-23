/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarAmpBasicAudioProcessorEditor::GuitarAmpBasicAudioProcessorEditor (GuitarAmpBasicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load IR");
    loadButton.onClick = [this]()
    {
        fileChooser = std::make_unique<juce::FileChooser>("Choose file", audioProcessor.root, "*");
        
        
        const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
                                      juce::FileBrowserComponent::canSelectFiles |
                                      juce::FileBrowserComponent::canSelectDirectories;
        
        fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            juce::File result (chooser.getResult());
            
            if(result.getFileExtension() == ".wav" | result.getFileExtension() == ".mp3")
            {
                audioProcessor.savedFile = result;
                audioProcessor.root = result.getParentDirectory().getFullPathName();
                audioProcessor.irLoader.reset();
                audioProcessor.irLoader.loadImpulseResponse(audioProcessor.savedFile, juce::dsp::Convolution::Stereo::yes,
                                                            juce::dsp::Convolution::Trim::yes, 0);
            }
        });
    };
}

GuitarAmpBasicAudioProcessorEditor::~GuitarAmpBasicAudioProcessorEditor()
{
}

//==============================================================================
void GuitarAmpBasicAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void GuitarAmpBasicAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    const auto btnX = getWidth() * 0.5f;
    const auto btnY = getHeight() * 0.5f;
    const auto btnWidth = getWidth() * 0.35;
    const auto btnHeight = getHeight() * 0.5f;
    
    loadButton.setBounds(btnX, btnY, btnWidth, btnHeight);
    
}
    
