/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JX11JAudioProcessorEditor::JX11JAudioProcessorEditor (JX11JAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), viewport(), contentComponent()
{
    juce::LookAndFeel::setDefaultLookAndFeel(&globalLNF);
    contentComponent.setSize(600, 1200);
    outputLevelKnob.label = "Level";
    contentComponent.addAndMakeVisible(outputLevelKnob);
    
    // Oscillators group
    oscTuneKnob.label = "Osc Tune";
    contentComponent.addAndMakeVisible(oscTuneKnob);
    oscFineKnob.label = "Osc Fine";
    contentComponent.addAndMakeVisible(oscFineKnob);
    oscMixKnob.label = "Osc Mix";
    contentComponent.addAndMakeVisible(oscMixKnob);
    // Filter group
    filterResoKnob.label = "Filter Resonace";
    contentComponent.addAndMakeVisible(filterResoKnob);
    polyModeButton.setButtonText("Poly");
    polyModeButton.setClickingTogglesState(true);
    contentComponent.addAndMakeVisible(polyModeButton);
    
    setResizable(true, false);
    addAndMakeVisible(viewport);
    //midi learn button
    midiLearnButton.setButtonText("MIDI Learn");
    midiLearnButton.addListener(this);
    contentComponent.addAndMakeVisible(midiLearnButton);
    viewport.setViewedComponent(&contentComponent, true);
    setSize (600, 300);
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
{                                      // x,y,w,h
    juce::Rectangle rectangleMiscGroup(20,20,100,120);
    outputLevelKnob.setBounds(rectangleMiscGroup);
    // Oscillator group
    juce::Rectangle rectangleOscillatorGroup(20,160,100,120);
    oscTuneKnob.setBounds(rectangleOscillatorGroup);
    rectangleOscillatorGroup = rectangleOscillatorGroup.withX(rectangleOscillatorGroup.getRight() + 20);
    oscFineKnob.setBounds(rectangleOscillatorGroup);
    rectangleOscillatorGroup = rectangleOscillatorGroup.withX(rectangleOscillatorGroup.getRight() + 20);
    oscMixKnob.setBounds(rectangleOscillatorGroup);
    
    
    // Filter group
    juce::Rectangle rectangleFilterGroup(20,360,100,120);
//    r = r.withX(r.getRight() + 20);
    filterResoKnob.setBounds(rectangleFilterGroup);
    
    polyModeButton.setSize(80, 30);
    
    

    // positions for buttons
    juce::Rectangle rb(400, 20, 100, 30);
    midiLearnButton.setBounds(rb);
    rb = rb.withY(rb.getY() + 40);
    polyModeButton.setBounds(rb);
    viewport.setBounds(getLocalBounds());
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
