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
    setSize (500, 600);
    
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
                
                audioProcessor.variableTree.setProperty("file1", result.getFullPathName(), nullptr);
                audioProcessor.variableTree.setProperty("root", result.getParentDirectory().getFullPathName(), nullptr);
                
                
                audioProcessor.irLoader.reset();
                audioProcessor.irLoader.loadImpulseResponse(audioProcessor.savedFile, juce::dsp::Convolution::Stereo::yes,
                                                            juce::dsp::Convolution::Trim::yes, 0);
                //irName.setText(result.getFileName(), juce::dontSendNotification);
            }
        });
    };
    irName.setText(audioProcessor.savedFile.getFileName(), juce::dontSendNotification);
    
    addAndMakeVisible(irName);
    
    addAndMakeVisible(sliderPreGain1);
    
    
    sliderPreGain1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPreGain1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPreGain1.setDoubleClickReturnValue(true, 0.0f);
    sliderPreGain1.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPreGain1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain1.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderPreGain1.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain1.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelPreGain1.attachToComponent(&sliderPreGain1, false);
    labelPreGain1.setText("PreGain1(dB)", juce::dontSendNotification);
    
    sliderPreGain1.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.preGainVal1 = sliderPreGain1.getValue();
    };
    
    addAndMakeVisible(sliderPreGain2);
    
    
    sliderPreGain2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPreGain2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPreGain2.setDoubleClickReturnValue(true, 0.0f);
    sliderPreGain2.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPreGain2.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain2.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderPreGain2.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain2.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelPreGain2.attachToComponent(&sliderPreGain2, false);
    labelPreGain2.setText("PreGain2(dB)", juce::dontSendNotification);
    
    sliderPreGain2.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.preGainVal2 = sliderPreGain2.getValue();
    };
    
    
    addAndMakeVisible(sliderPreGain3);
    
    sliderPreGain3.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPreGain3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPreGain3.setDoubleClickReturnValue(true, 0.0f);
    sliderPreGain3.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPreGain3.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain3.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderPreGain3.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain3.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelPreGain3.attachToComponent(&sliderPreGain3, false);
    labelPreGain3.setText("PreGain3(dB)", juce::dontSendNotification);
    
    sliderPreGain3.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.preGainVal3 = sliderPreGain3.getValue();
    };
    
    
    addAndMakeVisible(sliderPostGain);
    
    sliderPostGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPostGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPostGain.setDoubleClickReturnValue(true, 0.0f);
    sliderPostGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPostGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPostGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderPostGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPostGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelPostGain.attachToComponent(&sliderPostGain, false);
    labelPostGain.setText("PostGain(dB)", juce::dontSendNotification);
    
    sliderPostGain.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.postGainVal = sliderPostGain.getValue();
    };
    
    
    addAndMakeVisible(sliderPreEQ);
    
    sliderPreEQ.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPreEQ.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPreEQ.setDoubleClickReturnValue(true, 0.0f);
    sliderPreEQ.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPreEQ.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreEQ.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderPreEQ.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreEQ.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelPreEQ.attachToComponent(&sliderPreEQ, false);
    labelPreEQ.setText("PreEQ", juce::dontSendNotification);
    
    sliderPreEQ.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.preEQVal = sliderPreEQ.getValue();
    };
    
    
    
    addAndMakeVisible(waveshapeType1);

    labelWaveshapeType1.attachToComponent(&waveshapeType1, false);
    labelWaveshapeType1.setColour(juce::Label::textColourId, juce::Colours::white);
    labelWaveshapeType1.setText("Dist Type", juce::dontSendNotification);
    waveshapeType1.addItem("Tanh", 1);
    waveshapeType1.addItem("AmpTest", 2);
    waveshapeType1.addItem("x/abs(x)+1", 3);
    waveshapeType1.addItem("Atan", 4);
    waveshapeType1.addItem("HalfRect", 5);
    waveshapeType1.addItem("Amp1", 6);
    waveshapeType1.onChange = [this]{modeMenuChanged();};
    //waveshapeType1.setSelectedId(1);
    
    /*if(audioProcessor.waveshapeFunctionCurrent == "Tanh")
        waveshapeType1.setSelectedId(1);
    else if (audioProcessor.waveshapeFunctionCurrent == "AmpTest")
        waveshapeType1.setSelectedId(2);
    else if(audioProcessor.waveshapeFunctionCurrent == "x/abs(x)+1")
        waveshapeType1.setSelectedId(3);
    else if(audioProcessor.waveshapeFunctionCurrent == "Atan")
        waveshapeType1.setSelectedId(4);
    else if(audioProcessor.waveshapeFunctionCurrent == "HalfRect")
        waveshapeType1.setSelectedId(5);
    else if(audioProcessor.waveshapeFunctionCurrent == "Amp1")
        waveshapeType1.setSelectedId(6);*/
    //modeMenuChanged();
    
    
    addAndMakeVisible(sliderFilterHighGain);
    
    sliderFilterHighGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderFilterHighGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderFilterHighGain.setDoubleClickReturnValue(true, 0.0f);
    sliderFilterHighGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderFilterHighGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderFilterHighGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderFilterHighGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderFilterHighGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelFilterHighGain.attachToComponent(&sliderFilterHighGain, false);
    labelFilterHighGain.setText("Highs", juce::dontSendNotification);
    
    
    addAndMakeVisible(sliderFilterMidGain);
    
    sliderFilterMidGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderFilterMidGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderFilterMidGain.setDoubleClickReturnValue(true, 0.0f);
    sliderFilterMidGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderFilterMidGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderFilterMidGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderFilterMidGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderFilterMidGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelFilterMidGain.attachToComponent(&sliderFilterMidGain, false);
    labelFilterMidGain.setText("Mids", juce::dontSendNotification);
    
    addAndMakeVisible(sliderFilterLowGain);
    
    sliderFilterLowGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderFilterLowGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderFilterLowGain.setDoubleClickReturnValue(true, 0.0f);
    sliderFilterLowGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderFilterLowGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderFilterLowGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderFilterLowGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderFilterLowGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelFilterLowGain.attachToComponent(&sliderFilterLowGain, false);
    labelFilterLowGain.setText("Lows", juce::dontSendNotification);
    
    
    
   /* addAndMakeVisible(buttonWaveshapeToggle);
    buttonWaveshapeToggle.onClick() = [this] {
        updateToggleState (&buttonWaveshapeToggle,   "waveshapeToggle");
    };*/
    
    
    sliderAttachmentPreGain1 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREGAIN1", sliderPreGain1);
    sliderAttachmentPreGain2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREGAIN2", sliderPreGain2);
    
    sliderAttachmentPreGain3 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREGAIN3", sliderPreGain3);
    
    
    sliderAttachmentPostGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "POSTGAIN", sliderPostGain);
    
    sliderAttachmentPreEQ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREEQ", sliderPreEQ);
    
    comboAttachmentWaveshapeType1 = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, "TYPE", waveshapeType1);
    
    sliderAttachmentFilterHighGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "HIGH", sliderFilterHighGain);
    
    sliderAttachmentFilterMidGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "MID", sliderFilterMidGain);
    
    sliderAttachmentFilterLowGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW", sliderFilterLowGain);
}

GuitarAmpBasicAudioProcessorEditor::~GuitarAmpBasicAudioProcessorEditor()
{
}

/*void GuitarAmpBasicAudioProcessorEditor::updateToggleState (juce::Button* button, juce::String name)
{
    auto state = button->getToggleState();

//In our example we just post the toggle changes to the logger as they happen:
    juce::String stateString = state ? "ON" : "OFF";

    //juce::Logger::outputDebugString (name + " Button changed to " + stateString);
}*/



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
    
    /*const auto btnX = getWidth() * 0.35f;
    const auto btnY = getHeight() * 0.65f;
    const auto btnWidth = getWidth() * 0.15f;
    const auto btnHeight = getHeight() * 0.25f;
    int knobSize = 125;
    
    waveshapeType1.setBounds(getWidth()/2, getHeight()/2 - 175, 100, 25);
    loadButton.setBounds(btnX, btnY + 110, btnWidth, btnHeight);
    irName.setBounds(loadButton.getX() + loadButton.getWidth(), btnY, btnWidth * 2, btnHeight);
    
    sliderPreEQ.setBounds(getWidth()/2 - 200, getHeight()/2 - 125, knobSize, knobSize);
    
    sliderPreGain1.setBounds(getWidth()/2 - 50, getHeight()/2 - 125, knobSize, knobSize);
    sliderPreGain2.setBounds(getWidth()/2 - 50, getHeight()/2, knobSize, knobSize);
    sliderPostGain.setBounds(getWidth()/2 + 100, getHeight()/2 - 125, knobSize, knobSize);
    
    sliderFilterHighGain.setBounds(getWidth()/2 + 100, getHeight()/2, knobSize, knobSize);*/
    int leftOffest = 40;
    int topOffset = 40;
    int knobSize = 125;
    //IR load button/label
    loadButton.setBounds(leftOffest, topOffset, 100, 25);
    irName.setBounds(loadButton.getX(), loadButton.getY() - 20, 175, 25);
    
    //Waveshape menu
    waveshapeType1.setBounds(getWidth() - (loadButton.getWidth() + leftOffest), topOffset, 100, 25);
    
    sliderPreEQ.setBounds(getWidth() / 2 - 50, 100, knobSize, knobSize);
    sliderPostGain.setBounds(waveshapeType1.getX(), 100, knobSize, knobSize);
    
    sliderPreGain1.setBounds(loadButton.getX() / 2, 250, knobSize, knobSize);
    sliderPreGain2.setBounds(sliderPreEQ.getX(), 250, knobSize, knobSize);
    sliderPreGain3.setBounds(sliderPostGain.getX(), 250, knobSize, knobSize);
    
    sliderFilterLowGain.setBounds(sliderPreGain1.getX(), 400, knobSize, knobSize);
    sliderFilterMidGain.setBounds(sliderPreGain2.getX(), 400, knobSize, knobSize);
    sliderFilterHighGain.setBounds(sliderPreGain3.getX(), 400, knobSize, knobSize);
}

void GuitarAmpBasicAudioProcessorEditor::modeMenuChanged()
{
    switch (waveshapeType1.getSelectedId())
    {
        case 1:
            audioProcessor.waveshapeFunction = "Tanh";
            break;
        case 2:
            audioProcessor.waveshapeFunction = "AmpTest";
            break;
        case 3:
            audioProcessor.waveshapeFunction = "x/abs(x)+1";
            break;
        case 4:
            audioProcessor.waveshapeFunction = "Atan";
            break;
        case 5:
            audioProcessor.waveshapeFunction = "HalfRect";
            break;
        case 6:
            audioProcessor.waveshapeFunction = "Amp1";
            break;
            
        default:
            audioProcessor.waveshapeFunction = "Tanh";
            break;
    }
}
    
