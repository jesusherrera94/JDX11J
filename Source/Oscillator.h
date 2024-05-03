/*
  ==============================================================================

    Oscillator.h
    Created: 22 Apr 2024 9:48:25pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

const float TWO_PI = 6.2831853071795864f;

class Oscillator {
public:
    float amplitude;
    float inc;
    float phase;
    
    float freq;
    float sampleRate;
    float phaseBL;
    
    void reset() {
        phase = 0.0f;
        phaseBL = -0.5f;
    }
    // using additive synthesis method
    float nextBandLimitedSample() {
        phaseBL += inc;
        if (phaseBL >= 1.0f) {
            phaseBL -= 1.0f;
        }
        float output = 0.0f;
        float nyquist = sampleRate / 2.0f;
        float h = freq;
        float i = 1.0f;
        float m = 0.6366197724f; // this is 2/pi
        while (h < nyquist) {
            output += m * std::sin(TWO_PI * phaseBL * i) / i;
            h += freq;
            i += 1.0f;
            m = -m;
        }
        return output;
    }
    
    float nextSample() {

        return amplitude * nextBandLimitedSample();
    }
};

