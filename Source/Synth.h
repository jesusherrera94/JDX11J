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
