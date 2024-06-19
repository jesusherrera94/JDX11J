/*
  ==============================================================================

    old_filter.h
    Created: 18 Jun 2024 10:16:01pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once
// this is the manual filter!
class Filter {
    
public:
    float sampleRate;
    void updateCoefficients(float cutoff, float Q) {
        g = std::tan(PI * cutoff / sampleRate);
        k = 1.0f / Q;
        a1 = 1.0f / (1.0f + g * (g + k));
        a2 = g * a1;
        a3 = g * a2;
    }
    void reset() {
        g = 0.0f;
        k = 0.0f;
        a1 = 0.0f;
        a2 = 0.0f;
        a3 = 0.0f;
        
        icleq = 0.0f;
        ic2eq = 0.0f;
    }
    float render(float x) {
        float v3 = x - ic2eq;
        float v1 = a1 * icleq + a2 * v3;
        float v2 = ic2eq + a2 * icleq + a3 * v3;
        icleq = 2.0f * v1 - icleq;
        ic2eq = 2.0f * v2 - ic2eq;
        return v2;
    }
private:
    const float PI = 3.1415926535897932f;
    float g, k, a1, a2, a3; // filter cefficients
    float icleq, ic2eq; // internal state
};
