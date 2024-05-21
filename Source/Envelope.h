/*
  ==============================================================================

    Envelope.h
    Created: 17 May 2024 9:44:01pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

const float SILENCE = 0.0001f;

class Envelope {
public:
    float nextValue() {
        level *= multiplier;
        return level;
    }
    float level;
    float multiplier;
    
};
