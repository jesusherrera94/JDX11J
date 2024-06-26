/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"

using APVTS = juce::AudioProcessorValueTreeState;
using sliderAttachment = APVTS::SliderAttachment;

using ButtonAttachment = APVTS::ButtonAttachment;

//==============================================================================
/**
*/
class JX11JAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                   private juce::Button::Listener, juce::Timer
{
public:
    JX11JAudioProcessorEditor (JX11JAudioProcessor&);
    ~JX11JAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JX11JAudioProcessor& audioProcessor;
    // UI components
    // first add the attachment here!
    // then go to PluginEditor.cpp to complete the integration
    LookAndFeel globalLNF;
    RotaryKnob outputLevelKnob;
    sliderAttachment outputLevelAttachment { audioProcessor.apvts,
        ParameterID::outputLevel.getParamID(), outputLevelKnob.slider };
    RotaryKnob filterResoKnob;
    sliderAttachment filterResoAttachment { audioProcessor.apvts,
        ParameterID::filterReso.getParamID(), filterResoKnob.slider };
    juce::TextButton polyModeButton;
    ButtonAttachment polyModeAttachment { audioProcessor.apvts,
        ParameterID::polyMode.getParamID(), polyModeButton };
    
    // MIDI LEARN BUTTON
    juce::TextButton midiLearnButton;
    void buttonClicked(juce::Button* button) override;
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JX11JAudioProcessorEditor)
};
