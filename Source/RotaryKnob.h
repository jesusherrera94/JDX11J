/*
  ==============================================================================

    RotaryKnob.h
    Created: 24 Jun 2024 11:10:11pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotaryKnob  : public juce::Component
{
public:
    juce::Slider slider;
    juce::String label;
    RotaryKnob();
    ~RotaryKnob() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
