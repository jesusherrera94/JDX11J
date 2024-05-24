/*
  ==============================================================================

    Synth.h
    Created: 18 Apr 2024 9:04:44pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "NoiseGenerator.h"

class Synth {
public:
    Synth();
    float noiseMix; // param from UI
    float envDecay; // param from UI
    float envSustain; // param fron UI
    float envRelease; // param from UI
    float envAttack; // param from UI
    float oscMix; // param from UI
    float detune; // combination of UI params
    float tune; // combination of UI params
    
    // analog to prepareToPlate
    void allocateResources(double sampleRate, int samplesPerBlock);
    // analog to releaseResources
    void deallocateResources();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void reset();
    
private:
    float sampleRate;
    Voice voice;
    NoiseGenerator noiseGen;
    // methods
    void noteOn(int note, int velocity);
    void noteOff(int note);
};
