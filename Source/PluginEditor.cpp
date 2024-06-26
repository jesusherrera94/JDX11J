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
    setSize (600, 300);
    
    //midi learn button
    midiLearnButton.setButtonText("MIDI Learn");
    midiLearnButton.addListener(this);
    addAndMakeVisible(midiLearnButton);
}

JX11JAudioProcessorEditor::~JX11JAudioProcessorEditor()
{
    midiLearnButton.removeListener(this);
    audioProcessor.midiLearn = false;
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
    midiLearnButton.setBounds(400, 20, 100, 30);
}

void JX11JAudioProcessorEditor::buttonClicked(juce::Button* button) {
    button->setButtonText("Waiting...");
        button->setEnabled(false);
        audioProcessor.midiLearn = true;

        startTimerHz(10);
}

void JX11JAudioProcessorEditor::timerCallback() {
    if (!audioProcessor.midiLearn) {
        stopTimer();
        midiLearnButton.setButtonText("MIDI Learn");
        midiLearnButton.setEnabled(true);
    }
}
