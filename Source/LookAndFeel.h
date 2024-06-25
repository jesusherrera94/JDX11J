/*
  ==============================================================================

    LookAndFeel.h
    Created: 24 Jun 2024 11:30:32pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LookAndFeel: public juce::LookAndFeel_V4 {
public:
    LookAndFeel();
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel);
};
