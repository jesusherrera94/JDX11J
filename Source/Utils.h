/*
  ==============================================================================

    Utils.h
    Created: 18 Apr 2024 10:20:25pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

// util to protect volume peaks!!!!
inline void protectYourEars(float* buffer, int sampleCount) {
    if (buffer == nullptr) {
        return;
    }
    bool firstWarning = true;
    for (int i = 0; i < sampleCount; ++i) {
        float x = buffer[i];
        bool silence = false;
        if (std::isnan(x)) {
            DBG("!!! WARINING nan detected in audio buffer, silencing....!!!");
            silence = true;
        } else if (std::isinf(x)) {
            DBG("!!! WARINING inf detected in audio buffer, silencing....!!!");
            silence = true;
        } else if (x < -2.0f || x > 2.0f) { // screaming feedback
            DBG("!!! WARINING sample out of range, silencing....!!!");
            silence = true;
        } else if (x < -1.0f) {
            if (firstWarning) {
                DBG("!!! WARINING sample out of range, clamping....!!!");
                firstWarning = false;
            }
            buffer[i] = -1.0f;
        } else if (x > 1.0f) {
            if (firstWarning) {
                DBG("!!! WARINING sample out of range, clamping....!!!");
                firstWarning = false;
            }
            buffer[i] = 1.0f;
        }
        if (silence) {
            memset(buffer, 0, sampleCount * sizeof(float));
        }
    }
}
