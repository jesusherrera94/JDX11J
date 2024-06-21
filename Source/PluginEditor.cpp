/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JX11JAudioProcessorEditor::JX11JAudioProcessorEditor (JX11JAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    outputLevelKnob.setSliderStyle(
                                   juce::Slider::SliderStyle::RotaryHorizontalDrag
                                   );
    outputLevelKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    addAndMakeVisible(outputLevelKnob);
    setSize (400, 300);
}

JX11JAudioProcessorEditor::~JX11JAudioProcessorEditor()
{
}

//==============================================================================
// This is the original UI provided by JUCE!!!!
void JX11JAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void JX11JAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    outputLevelKnob.setBounds(20, 20, 100, 120);
}
