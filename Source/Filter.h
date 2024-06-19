/*
  ==============================================================================

    Filter.h
    Created: 17 Jun 2024 9:03:55pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Filter : public juce::dsp::LadderFilter<float> {
public:
    void updateCoefficients(float cutoff, float Q) {
        setCutoffFrequencyHz(cutoff);
        setResonance(std::clamp(Q / 30.0f, 0.0f, 1.0f));
    }
    float render(float x) {
        updateSmoothers();
        return processSample(x, 0);
    }
};
