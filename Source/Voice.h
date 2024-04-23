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
    void reset() {
        note = 0;
        osc.reset();
    }
    float render() {
        return osc.nextSample();
    }
};
