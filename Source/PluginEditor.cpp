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
    juce::LookAndFeel::setDefaultLookAndFeel(&globalLNF);
    outputLevelKnob.label = "Level";
    addAndMakeVisible(outputLevelKnob);
    filterResoKnob.label = "Reso";
    addAndMakeVisible(filterResoKnob);
    polyModeButton.setButtonText("Poly");
    polyModeButton.setClickingTogglesState(true);
    addAndMakeVisible(polyModeButton);
    
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
    juce::Rectangle r(20,20,100,120);
    outputLevelKnob.setBounds(r);
    r = r.withX(r.getRight() + 20);
    filterResoKnob.setBounds(r);
    polyModeButton.setSize(80, 30);
    r = r.withY(r.getBottom());
    polyModeButton.setCentrePosition({r.getX(), r.getCentreY()});
}
