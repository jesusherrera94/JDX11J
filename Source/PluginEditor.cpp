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
    contentComponent.setSize(520, 1150);
    outputLevelKnob.label = "Level";
    contentComponent.addAndMakeVisible(outputLevelKnob);
    noiseKnob.label = "Noise";
    contentComponent.addAndMakeVisible(noiseKnob);
    octaveKnob.label = "Octave";
    contentComponent.addAndMakeVisible(octaveKnob);
    
    // Oscillators group
    oscTuneKnob.label = "Osc Tune";
    contentComponent.addAndMakeVisible(oscTuneKnob);
    oscFineKnob.label = "Osc Fine";
    contentComponent.addAndMakeVisible(oscFineKnob);
    oscMixKnob.label = "Osc Mix";
    contentComponent.addAndMakeVisible(oscMixKnob);
    // Glide group
    glideBendKnob.label = "Glide Bend";
    contentComponent.addAndMakeVisible(glideBendKnob);
    glideRateKnob.label = "Glide Rate";
    contentComponent.addAndMakeVisible(glideRateKnob);
    // Filter group 1
    filterFreqKnob.label = "Filter Freq";
    contentComponent.addAndMakeVisible(filterFreqKnob);
    filterResoKnob.label = "Filter Resonace";
    contentComponent.addAndMakeVisible(filterResoKnob);
    filterEnvKnob.label = "Filter Env";
    contentComponent.addAndMakeVisible(filterEnvKnob);
    // Filter group 2
    filterLFOKnob.label = "Filter LFO";
    contentComponent.addAndMakeVisible(filterLFOKnob);
    filterVelocityKnob.label = "Filter Velocity";
    contentComponent.addAndMakeVisible(filterVelocityKnob);
    filterAttackKnob.label = "Filter Attack";
    contentComponent.addAndMakeVisible(filterAttackKnob);
    // Filter group 3
    filterDecayKnob.label = "Filter Decay";
    contentComponent.addAndMakeVisible(filterDecayKnob);
    filterSustainKnob.label = "Filter Sustain";
    contentComponent.addAndMakeVisible(filterSustainKnob);
    filterReleaseKnob.label = "Filter Release";
    contentComponent.addAndMakeVisible(filterReleaseKnob);
    // Envelope group
    envAttackKnob.label= "Env Attack";
    contentComponent.addAndMakeVisible(envAttackKnob);
    envDecayKnob.label = "Env Decay";
    contentComponent.addAndMakeVisible(envDecayKnob);
    envSustainKnob.label = "Env Sustain";
    contentComponent.addAndMakeVisible(envSustainKnob);
    // Envelope, LFO Rate and Vibrato group
    envReleaseKnob.label = "Env Release";
    contentComponent.addAndMakeVisible(envReleaseKnob);
    LFORateKnob.label = "LFO Rate";
    contentComponent.addAndMakeVisible(LFORateKnob);
    vibratoKnob.label = "Vibrato";
    contentComponent.addAndMakeVisible(vibratoKnob);
    
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
    setSize (530, 300);
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
    rectangleMiscGroup = rectangleMiscGroup.withX(rectangleMiscGroup.getRight() + 20);
    noiseKnob.setBounds(rectangleMiscGroup);
    rectangleMiscGroup = rectangleMiscGroup.withX(rectangleMiscGroup.getRight() + 20);
    octaveKnob.setBounds(rectangleMiscGroup);
    
    // Oscillator group
    juce::Rectangle rectangleOscillatorGroup(20,160,100,120);
    oscTuneKnob.setBounds(rectangleOscillatorGroup);
    rectangleOscillatorGroup = rectangleOscillatorGroup.withX(rectangleOscillatorGroup.getRight() + 20);
    oscFineKnob.setBounds(rectangleOscillatorGroup);
    rectangleOscillatorGroup = rectangleOscillatorGroup.withX(rectangleOscillatorGroup.getRight() + 20);
    oscMixKnob.setBounds(rectangleOscillatorGroup);
    // Glide group
    juce::Rectangle rectangleGlideGroup(20,300,100,120);
    glideBendKnob.setBounds(rectangleGlideGroup);
    rectangleGlideGroup = rectangleGlideGroup.withX(rectangleGlideGroup.getRight() + 20);
    glideRateKnob.setBounds(rectangleGlideGroup);
    // Filter group 1
    juce::Rectangle rectangleFilterGroup1(20,440,100,120);
    filterFreqKnob.setBounds(rectangleFilterGroup1);
    rectangleFilterGroup1 = rectangleFilterGroup1.withX(rectangleFilterGroup1.getRight() + 20);
    filterResoKnob.setBounds(rectangleFilterGroup1);
    rectangleFilterGroup1 = rectangleFilterGroup1.withX(rectangleFilterGroup1.getRight() + 20);
    filterEnvKnob.setBounds(rectangleFilterGroup1);
    // Filter group 2
    juce::Rectangle rectangleFilterGroup2(20,580,100,120);
    filterLFOKnob.setBounds(rectangleFilterGroup2);
    rectangleFilterGroup2 = rectangleFilterGroup2.withX(rectangleFilterGroup2.getRight() + 20);
    filterVelocityKnob.setBounds(rectangleFilterGroup2);
    rectangleFilterGroup2 = rectangleFilterGroup2.withX(rectangleFilterGroup2.getRight() + 20);
    filterAttackKnob.setBounds(rectangleFilterGroup2);
    // Filter group 3
    juce::Rectangle rectangleFilterGroup3(20,720,100,120);
    filterDecayKnob.setBounds(rectangleFilterGroup3);
    rectangleFilterGroup3 = rectangleFilterGroup3.withX(rectangleFilterGroup3.getRight() + 20);
    filterSustainKnob.setBounds(rectangleFilterGroup3);
    rectangleFilterGroup3 = rectangleFilterGroup3.withX(rectangleFilterGroup3.getRight() + 20);
    filterReleaseKnob.setBounds(rectangleFilterGroup3);
    // Envelope group
    juce::Rectangle rectangleEnvGroup(20,860,100,120);
    envAttackKnob.setBounds(rectangleEnvGroup);
    rectangleEnvGroup = rectangleEnvGroup.withX(rectangleEnvGroup.getRight() + 20);
    envDecayKnob.setBounds(rectangleEnvGroup);
    rectangleEnvGroup = rectangleEnvGroup.withX(rectangleEnvGroup.getRight() + 20);
    envSustainKnob.setBounds(rectangleEnvGroup);
    // Envelope, LFO Rate and Vibrato group
    juce::Rectangle rectangleELVGroup(20,1000,100,120);
    envReleaseKnob.setBounds(rectangleELVGroup);
    rectangleELVGroup = rectangleELVGroup.withX(rectangleELVGroup.getRight() + 20);
    LFORateKnob.setBounds(rectangleELVGroup);
    rectangleELVGroup = rectangleELVGroup.withX(rectangleELVGroup.getRight() + 20);
    vibratoKnob.setBounds(rectangleELVGroup);
    
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
