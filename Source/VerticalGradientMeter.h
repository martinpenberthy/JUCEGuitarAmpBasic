/*
  ==============================================================================

    VerticalGradientMeter.h
    Created: 9 Jun 2023 3:21:20pm
    Author:  Martin Penberthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Gui {

class VerticalGradientMeter : public juce::Component, public juce::Timer
{
public:
    VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
    {
        startTimerHz(24);
    }
    
    void paint(juce::Graphics &g) override
    {
        const auto level = valueSupplier();
        
        auto bounds = getLocalBounds().toFloat();
        
        
        g.setColour(juce::Colours::black);
        g.fillRect(bounds);
        
        g.setGradientFill(gradient);
        const auto scaledY = juce::jmap(level, -60.0f, 6.0f, 0.0f, static_cast<float>(getHeight()));
        g.fillRect(bounds.removeFromBottom(scaledY));
        
        auto currLevelBounds = getLocalBounds().toFloat();
        currLevelBounds.setX(currLevelBounds.getX() + 100);
        //new juce::Rectangle<float>(100.0f, 100.0f, 40.0f, 20.0f);
        /*bounds.setX(100);
        bounds.setY(100);
        bounds.setWidth(40);s
        bounds.setHeight(25);*/
        
        /*g.setFont (30.0f);
        g.drawText(std::to_string(level), 250.0f, 260.0f, 100.0f, 100.0f, juce::Justification::centred, true);*/
    }
    
    void resized() override
    {
        const auto bounds = getLocalBounds().toFloat();
        gradient = juce::ColourGradient{
            juce::Colours::green,
            bounds.getBottomLeft(),
            juce::Colours::red,
            bounds.getTopLeft(),
            false
        };
        gradient.addColour(0.5f, juce::Colours::yellow);
    }
    
    void setLevel(const float value)
    {
        level = value;
    }
    
    void timerCallback() override
    {
        repaint();
    }
    
private:
    std::function<float()> valueSupplier;
    juce::ColourGradient gradient{};
    float level = -60.0f;

};
}
