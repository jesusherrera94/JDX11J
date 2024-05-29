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
    Oscillator osc1;
    Oscillator osc2;
    float saw;
    Envelope env;
    float period;
    float panLeft, panRight;
    void reset() {
        note = 0;
        saw = 0.0f;
        panLeft = 0.707f;
        panRight = 0.707f;
        osc1.reset();
        osc2.reset();
        env.reset();
    }
    float render(float input) {
        float sample1 = osc1.nextSample();
        float sample2 = osc2.nextSample();
        saw = saw * 0.997f + sample1 - sample2; // multipliying by 0.997 act as low pass filter
        // saw = saw * 0.997f - sample; it can be minus it only flips the signal 180 degrees
        
        float output = saw + input;
        
        // including envelope
        float envelope = env.nextValue();
        return output * envelope;
    }
    void release() {
        env.release();
    }
    void updatePanning() {
        float panning = std::clamp((note - 60.0f) / 24.0f, -1.0f, 1.0f);
        panLeft = std::sin(PI_OVER_4 * (1.0f - panning));
        panRight = std::sin(PI_OVER_4 * (1.0 + panning));
    }
};
