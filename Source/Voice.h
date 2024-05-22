/*
  ==============================================================================

    Voice.h
    Created: 18 Apr 2024 9:05:31pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once
#include "Oscillator.h"
#include "Envelope.h"

struct Voice {
    int note;
    Oscillator osc;
    float saw;
    Envelope env;
    void reset() {
        note = 0;
        saw = 0.0f;
        osc.reset();
        env.reset();
    }
    float render(float input) {
        float sample = osc.nextSample();
        saw = saw * 0.997f + sample; // multipliying by 0.997 act as low pass filter
        // saw = saw * 0.997f - sample; it can be minus it only flips the signal 180 degrees
        
        float output = saw + input;
        
        // including envelope
        float envelope = env.nextValue();
        return output * envelope;
    }
    void release() {
        env.release();
    }
};
