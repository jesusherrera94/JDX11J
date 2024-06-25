/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 24 Jun 2024 11:30:32pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#include "LookAndFeel.h"

LookAndFeel::LookAndFeel() {
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(26,23,31));
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0,0,0));
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(233,53,37));
    setColour(juce::Slider::thumbColourId, juce::Colour(255,255,255));
}
