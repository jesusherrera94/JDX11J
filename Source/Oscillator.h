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
    
    void reset() {
        phase = 1.5707963268f;
        // optimal and faster solution
        sin0 = amplitude * std::sin(phase * TWO_PI);
        sin1 = amplitude * std::sin((phase - inc) * TWO_PI);
        dsin = 2.0f * std::cos(inc * TWO_PI);
    }
    float nextSample() {
            // frecuency formula
        // Old implementation
//        float output = amplitude * std::sin( TWO_PI * sampleIndex * freq / sampleRate + phaseOffset);
//        sampleIndex += 1;
//        return output;
        // option two but expensive
//        phase += inc;
//        if (phase >= 1.0f) {
//            phase -= 1.0f;
//        }
//        return amplitude * std::sin(TWO_PI * phase);
        // optimal and faster solution
        float sinx = dsin * sin0 - sin1;
        sin1 = sin0;
        sin0 = sinx;
        return sinx;
    }
private:
    float sin0;
    float sin1;
    float dsin;
};

