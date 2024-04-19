/*
  ==============================================================================

    Voice.h
    Created: 18 Apr 2024 9:05:31pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

struct Voice {
    int note;
    int velocity;
    void reset() {
        note = 0;
        velocity = 0;
    }
};
