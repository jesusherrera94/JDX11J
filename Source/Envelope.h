/*
  ==============================================================================

    Envelope.h
    Created: 17 May 2024 9:44:01pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

class Envelope {
public:
    float nextValue() {
        level *= 0.9999f;
        return level;
    }
    float level;
};
