/*
  ==============================================================================

    NoiseGenerator.h
    Created: 18 Apr 2024 9:51:18pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

class NoiseGenerator {
public:
    void reset() {
        noiseSeed = 22222;
    }
    float nextValue() {
        // Generate the next integer pseudorandom number
        noiseSeed = noiseSeed * 196314165 + 907633515;
        // convert to a signed value
        int temp = int(noiseSeed >> 7 ) - 16777216;
        
        // Convert to a floating-point number between -1.0 to 1.0
        return float(temp) / 16777216.0f;
    }
private:
    unsigned int noiseSeed;
};
