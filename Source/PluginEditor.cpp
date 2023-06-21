/*
  ==============================================================================
    Martin Penberthy
    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarAmpBasicAudioProcessorEditor::GuitarAmpBasicAudioProcessorEditor (GuitarAmpBasicAudioProcessor& p)
    : AudioProcessorEditor (&p),
      verticalGradientMeterInput([&]() {return audioProcessor.getRMSValueInput(0); }),
      verticalGradientMeterOutput([&]() {return audioProcessor.getRMSValueOutput(0); }),
      audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 520);
    
    
    addAndMakeVisible(verticalGradientMeterInput);
    addAndMakeVisible(verticalGradientMeterOutput);
    startTimerHz(24);
    
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load IR");
    loadButton.onClick = [this]()
    {
        fileLoader();
    };
    irName.setText(audioProcessor.savedFile.getFileName(), juce::dontSendNotification);
    addAndMakeVisible(irName);  
    

    addAndMakeVisible(waveshapeType1);

    labelWaveshapeType1.attachToComponent(&waveshapeType1, false);
    labelWaveshapeType1.setColour(juce::Label::textColourId, juce::Colours::white);
    labelWaveshapeType1.setText("Dist Type", juce::dontSendNotification);
    waveshapeType1.addItem("Tanh", 1);
    waveshapeType1.addItem("Amp2", 2);
    waveshapeType1.addItem("x/abs(x)+1", 3);
    waveshapeType1.addItem("Atan", 4);
    waveshapeType1.addItem("HalfRect", 5);
    waveshapeType1.addItem("Amp1", 6);
    waveshapeType1.onChange = [this]{modeMenuChanged();};

    //modeMenuChanged();
    
    lookAndFeel.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    lookAndFeel.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    lookAndFeel.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    lookAndFeel.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    lookAndFeel.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    
    addAndMakeVisible(sliderInputGain);
    
    setSliderProperties(&sliderInputGain);
    sliderInputGain.setLookAndFeel(&lookAndFeel);
    labelInputGain.attachToComponent(&sliderInputGain, false);
    labelInputGain.setText("InputGain(dB)", juce::dontSendNotification);
    
    addAndMakeVisible(sliderPreGain1);
    
    setSliderProperties(&sliderPreGain1);
    sliderPreGain1.setLookAndFeel(&lookAndFeel);
    labelPreGain1.attachToComponent(&sliderPreGain1, false);
    labelPreGain1.setText("PreGain1(dB)", juce::dontSendNotification);
    
    addAndMakeVisible(sliderPreGain2);
    
    setSliderProperties(&sliderPreGain2);
    sliderPreGain2.setLookAndFeel(&lookAndFeel);
    labelPreGain2.attachToComponent(&sliderPreGain2, false);
    labelPreGain2.setText("PreGain2(dB)", juce::dontSendNotification);
    
    addAndMakeVisible(sliderPreGain3);
    
    setSliderProperties(&sliderPreGain3);
    sliderPreGain3.setLookAndFeel(&lookAndFeel);
    labelPreGain3.attachToComponent(&sliderPreGain3, false);
    labelPreGain3.setText("PreGain3(dB)", juce::dontSendNotification);
    
    addAndMakeVisible(sliderPostGain);
    
    setSliderProperties(&sliderPostGain);
    sliderPostGain.setLookAndFeel(&lookAndFeel);
    labelPostGain.attachToComponent(&sliderPostGain, false);
    labelPostGain.setText("PostGain(dB)", juce::dontSendNotification);
    
    addAndMakeVisible(sliderPreEQ);
    
    setSliderProperties(&sliderPreEQ);
    sliderPreEQ.setLookAndFeel(&lookAndFeel);
    labelPreEQ.attachToComponent(&sliderPreEQ, false);
    labelPreEQ.setText("PreEQ", juce::dontSendNotification);

    addAndMakeVisible(sliderFilterHighGain);
    
    setSliderProperties(&sliderFilterHighGain);
    sliderFilterHighGain.setLookAndFeel(&lookAndFeel);
    labelFilterHighGain.attachToComponent(&sliderFilterHighGain, false);
    labelFilterHighGain.setText("Highs", juce::dontSendNotification);
    
    addAndMakeVisible(sliderFilterMidGain);
    
    setSliderProperties(&sliderFilterMidGain);
    sliderFilterMidGain.setLookAndFeel(&lookAndFeel);
    labelFilterMidGain.attachToComponent(&sliderFilterMidGain, false);
    labelFilterMidGain.setText("Mids", juce::dontSendNotification);
    
    addAndMakeVisible(sliderFilterLowGain);
    
    setSliderProperties(&sliderFilterLowGain);
    sliderFilterLowGain.setLookAndFeel(&lookAndFeel);
    labelFilterLowGain.attachToComponent(&sliderFilterLowGain, false);
    labelFilterLowGain.setText("Lows", juce::dontSendNotification);
    
    makeSliderAttachments();
}

GuitarAmpBasicAudioProcessorEditor::~GuitarAmpBasicAudioProcessorEditor()
{
}


void GuitarAmpBasicAudioProcessorEditor::makeSliderAttachments()
{
    sliderAttachmentInputGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "INPUTGAIN", sliderInputGain);
    sliderAttachmentPostGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "POSTGAIN", sliderPostGain);
    sliderAttachmentPreEQ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREEQ", sliderPreEQ);
    
    sliderAttachmentPreGain1 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREGAIN1", sliderPreGain1);
    sliderAttachmentPreGain2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREGAIN2", sliderPreGain2);
    sliderAttachmentPreGain3 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "PREGAIN3", sliderPreGain3);
    
    sliderAttachmentFilterHighGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "HIGH", sliderFilterHighGain);
    sliderAttachmentFilterMidGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "MID", sliderFilterMidGain);
    sliderAttachmentFilterLowGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "LOW", sliderFilterLowGain);
    
    comboAttachmentWaveshapeType1 = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, "TYPE1", waveshapeType1);
}

void GuitarAmpBasicAudioProcessorEditor::timerCallback()
{
    
    if(audioProcessor.isInput)
    {
        verticalGradientMeterInput.setLevel(audioProcessor.getRMSValueInput(0));
        verticalGradientMeterInput.repaint();
    }else
    {
        verticalGradientMeterOutput.setLevel(audioProcessor.getRMSValueOutput(0));
        verticalGradientMeterOutput.repaint();
    }
    
}

void GuitarAmpBasicAudioProcessorEditor::setSliderProperties(juce::Slider *sliderToSet)
{
    
    sliderToSet->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderToSet->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderToSet->setDoubleClickReturnValue(true, 0.0f);
}

void GuitarAmpBasicAudioProcessorEditor::fileLoader()
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
            irName.setText(result.getFileName(), juce::dontSendNotification);
            //irName.setText(audioProcessor.savedFile.getFileName(), juce::dontSendNotification);

        }
    });
}

//==============================================================================
void GuitarAmpBasicAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    /*testImage = juce::ImageCache::getFromMemory(BinaryData::Glitchtastic_jpg, BinaryData::Glitchtastic_jpgSize);
    g.drawImageWithin(testImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);*/
     
    /*g.setColour (juce::Colours::whitesmoke.withAlpha(0.25f));
 
    float lineLength = 15.0f;
    juce::Line<float> line (juce::Point<float> (verticalGradientMeterInput.getX() + verticalGradientMeterInput.getWidth(),
                                                verticalGradientMeterInput.getY() + lineLength),
                            juce::Point<float> (verticalGradientMeterInput.getX() + verticalGradientMeterInput.getWidth() + lineLength,
                                                verticalGradientMeterInput.getY() + lineLength));
 
    g.drawLine (line, 2.0f);*/
        
   /* levelMeterInputLines = juce::ImageCache::getFromMemory(BinaryData::LevelMeterLines_png, BinaryData::LevelMeterLines_pngSize);
    levelMeterInputLines.multiplyAllAlphas(0.75f);
    g.drawImageWithin(levelMeterInputLines,
                      verticalGradientMeterInput.getX() + verticalGradientMeterInput.getWidth(), //X
                      verticalGradientMeterInput.getY(), //Y
                      verticalGradientMeterInput.getWidth() * 2, //Width
                      verticalGradientMeterInput.getHeight(), //Height
                      juce::RectanglePlacement::stretchToFit);
    
    levelMeterInputNums = juce::ImageCache::getFromMemory(BinaryData::LevelMeterNums_png, BinaryData::LevelMeterNums_pngSize);
    levelMeterInputNums.multiplyAllAlphas(0.75f);
    g.drawImageWithin(levelMeterInputNums,
                      verticalGradientMeterInput.getX() + verticalGradientMeterInput.getWidth(), //X
                      verticalGradientMeterInput.getY(), //Y
                      verticalGradientMeterInput.getWidth() * 2, //Width
                      verticalGradientMeterInput.getHeight(), //Height
                      juce::RectanglePlacement::stretchToFit);*/
}

void GuitarAmpBasicAudioProcessorEditor::resized()
{
    int meterWidth = 15;
    int meterHeight = 510;
    int meterX = 5;
    int meterY = 5;
    verticalGradientMeterInput.setBounds(meterX, meterY, meterWidth, meterHeight);
    verticalGradientMeterOutput.setBounds(getWidth() - (verticalGradientMeterInput.getWidth() + verticalGradientMeterInput.getX()), meterY, meterWidth, meterHeight);
        
    int leftOffest = 60;
    int topOffset = 40;
    int knobSize = 125;
    int row1Y = 100;
    int row2Y = 250;
    int row3Y = 400;
    
    //IR load button/label
    loadButton.setBounds(leftOffest, topOffset, 100, 25);
    irName.setBounds(loadButton.getX(), loadButton.getY() - 20, 175, 25);
    
    //Waveshape menu
    waveshapeType1.setBounds(getWidth() - (loadButton.getWidth() + leftOffest + 10), topOffset, 100, 25);
    
    //Top row
    sliderInputGain.setBounds(loadButton.getX(), row1Y, knobSize, knobSize);
    sliderPreEQ.setBounds(getWidth() / 2 - 50, row1Y, knobSize, knobSize);
    sliderPostGain.setBounds(waveshapeType1.getX(), row1Y, knobSize, knobSize);
    
    //Middle row
    sliderPreGain1.setBounds(loadButton.getX(), row2Y, knobSize, knobSize);
    sliderPreGain2.setBounds(sliderPreEQ.getX(), row2Y, knobSize, knobSize);
    sliderPreGain3.setBounds(sliderPostGain.getX(), row2Y, knobSize, knobSize);
    
    //Bottom row
    sliderFilterLowGain.setBounds(loadButton.getX(), row3Y, knobSize, knobSize);
    sliderFilterMidGain.setBounds(sliderPreGain2.getX(), row3Y, knobSize, knobSize);
    sliderFilterHighGain.setBounds(sliderPreGain3.getX(), row3Y, knobSize, knobSize);
}

void GuitarAmpBasicAudioProcessorEditor::modeMenuChanged()
{
    switch (waveshapeType1.getSelectedId())
    {
        case 1:
            audioProcessor.waveshapeFunction = "Tanh";
            break;
        case 2:
            audioProcessor.waveshapeFunction = "Amp2";
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
    
