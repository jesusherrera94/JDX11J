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
using ComboBoxAttachment = APVTS::ComboBoxAttachment;

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
    juce::Viewport viewport;
    juce::Component contentComponent;
    // output knob
    RotaryKnob outputLevelKnob;
    sliderAttachment outputLevelAttachment { audioProcessor.apvts,
        ParameterID::outputLevel.getParamID(), outputLevelKnob.slider };
    RotaryKnob noiseKnob;
    sliderAttachment noiseAttachment { audioProcessor.apvts,
        ParameterID::noise.getParamID(), noiseKnob.slider };
    RotaryKnob octaveKnob;
    sliderAttachment octaveAttachment { audioProcessor.apvts,
        ParameterID::octave.getParamID(), octaveKnob.slider };

    
    // =============================== OSCILLATOR GROUP ==========================
    RotaryKnob oscTuneKnob;
    sliderAttachment oscTuneAttachment { audioProcessor.apvts,
        ParameterID::oscTune.getParamID(), oscTuneKnob.slider };
    RotaryKnob oscFineKnob;
    sliderAttachment oscFineAttachment { audioProcessor.apvts,
        ParameterID::oscFine.getParamID(), oscFineKnob.slider };
    RotaryKnob oscMixKnob;
    sliderAttachment oscMixAttachment { audioProcessor.apvts,
        ParameterID::oscMix.getParamID(), oscMixKnob.slider };
    
    // ================================= GLIDE GROUP =============================
    // TODO: Implement glide mode option
    RotaryKnob glideRateKnob;
    sliderAttachment glideRateAttachment { audioProcessor.apvts,
        ParameterID::glideRate.getParamID(), glideRateKnob.slider };
    RotaryKnob glideBendKnob;
    sliderAttachment glideBendAttachment { audioProcessor.apvts,
        ParameterID::glideBend.getParamID(), glideBendKnob.slider };
    
    // ================================== FILTER GROUP 1 ============================
    // filter Reso knob
    RotaryKnob filterResoKnob;
    sliderAttachment filterResoAttachment { audioProcessor.apvts,
        ParameterID::filterReso.getParamID(), filterResoKnob.slider };
    RotaryKnob filterFreqKnob;
    sliderAttachment filterFreqAttachment { audioProcessor.apvts,
        ParameterID::filterFreq.getParamID(), filterFreqKnob.slider };
    RotaryKnob filterEnvKnob;
    sliderAttachment filterEnvAttachment { audioProcessor.apvts,
        ParameterID::filterEnv.getParamID(), filterEnvKnob.slider };
    
    // ================================== FILTER GROUP 2 ============================
    RotaryKnob filterLFOKnob;
    sliderAttachment filterLFOAttachment { audioProcessor.apvts,
        ParameterID::filterLFO.getParamID(), filterLFOKnob.slider };
    RotaryKnob filterVelocityKnob;
    sliderAttachment filterVelocityAttachment { audioProcessor.apvts,
        ParameterID::filterVelocity.getParamID(), filterVelocityKnob.slider };
    RotaryKnob filterAttackKnob;
    sliderAttachment filterAttackAttachment { audioProcessor.apvts,
        ParameterID::filterAttack.getParamID(), filterAttackKnob.slider };
    
    // ================================== FILTER GROUP 3 ============================
    RotaryKnob filterDecayKnob;
    sliderAttachment filterDecayAttachment { audioProcessor.apvts,
        ParameterID::filterDecay.getParamID(), filterDecayKnob.slider };
    RotaryKnob filterSustainKnob;
    sliderAttachment filterSustainAttachment { audioProcessor.apvts,
        ParameterID::filterSustain.getParamID(), filterSustainKnob.slider };
    RotaryKnob filterReleaseKnob;
    sliderAttachment filterReleaseAttachment { audioProcessor.apvts,
        ParameterID::filterRelease.getParamID(), filterReleaseKnob.slider };
    
    // ================================== ENVELOPE GROUP ============================
    RotaryKnob envAttackKnob;
    sliderAttachment envAttackAttachment { audioProcessor.apvts,
        ParameterID::envAttack.getParamID(), envAttackKnob.slider };
    RotaryKnob envDecayKnob;
    sliderAttachment envDecayAttachment { audioProcessor.apvts,
        ParameterID::envDecay.getParamID(), envDecayKnob.slider };
    RotaryKnob envSustainKnob;
    sliderAttachment envSustainAttachment { audioProcessor.apvts,
        ParameterID::envSustain.getParamID(), envSustainKnob.slider };
    
    // ================= ENVELOPE, LFO RATE, VIBRATO GROUP ======================
    RotaryKnob envReleaseKnob;
    sliderAttachment envReleaseAttachment { audioProcessor.apvts,
        ParameterID::envRelease.getParamID(), envReleaseKnob.slider };
    RotaryKnob LFORateKnob;
    sliderAttachment LFORateAttachment { audioProcessor.apvts,
        ParameterID::lfoRate.getParamID(), LFORateKnob.slider };
    RotaryKnob vibratoKnob;
    sliderAttachment vibratoAttachment { audioProcessor.apvts,
        ParameterID::vibrato.getParamID(), vibratoKnob.slider };
    
    // ========================== RIGHT CONTROL BUTTONS =========================
    
    // polymode toggle button
    juce::TextButton polyModeButton;
    ButtonAttachment polyModeAttachment { audioProcessor.apvts,
        ParameterID::polyMode.getParamID(), polyModeButton };
    
    // MIDI LEARN BUTTON
    juce::TextButton midiLearnButton;
    void buttonClicked(juce::Button* button) override;
    void timerCallback() override;
    juce::Label glideModeLabel;
    juce::ComboBox glideModeComboBox;
    ComboBoxAttachment glideModeAttachment { audioProcessor.apvts,
    ParameterID::glideMode.getParamID(), glideModeComboBox };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JX11JAudioProcessorEditor)
};
