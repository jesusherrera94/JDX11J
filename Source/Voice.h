/*
  ==============================================================================

    Voice.h
    Created: 18 Apr 2024 9:05:31pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once
#include "Oscillator.h"

struct Voice {
    int note;
    Oscillator osc;
    float saw;
    void reset() {
        note = 0;
        saw = 0.0f;
        osc.reset();
    }
    float render() {
        float sample = osc.nextSample();
        saw = saw * 0.997f + sample; // multipliying by 0.997 act as low pass filter
        // saw = saw * 0.997f - sample; it can be minus it only flips the signal 180 degrees
        return saw;
    }
};
