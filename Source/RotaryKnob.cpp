/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 24 Jun 2024 11:10:11pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"

//==============================================================================
static constexpr int labelHeight = 15;
static constexpr int textBoxHeight = 15;
RotaryKnob::RotaryKnob()
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, textBoxHeight);
    addAndMakeVisible(slider);
    setBounds(0, 0, 100, 120);

}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    auto bounds = getLocalBounds();
    g.drawText (label, juce::Rectangle<int>{0, 0, bounds.getWidth(), labelHeight},
        juce::Justification::centred);   // draw some placeholder text
}

void RotaryKnob::resized()
{
    auto bounds = getLocalBounds();
    slider.setBounds(0, labelHeight, bounds.getWidth(), bounds.getHeight() - labelHeight);

}
