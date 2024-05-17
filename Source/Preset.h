/*
  ==============================================================================

    Preset.h
    Created: 16 May 2024 9:40:31pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

#include <cstring>

const int NUM_PARAMS = 26;

struct Preset {
    Preset(const char* name,
           float p0, float p1, float p2, float p3,
           float p4, float p5, float p6, float p7,
           float p8, float p9, float p10, float p11,
           float p12, float p13, float p14, float p15,
           float p16, float p17, float p18, float p19,
           float p20, float p21, float p22, float p23,
           float p24, float p25) {
        strcpy(this->name, name);
        param[0] = p0; // osc mix
        param[1] = p1; // osc tune
        param[2] = p2; // osc fine
        param[3] = p3; // glide mode
        param[4] = p4; // glide rate
        param[5] = p5; // glide bend
        param[6] = p6; // filter freq
        param[7] = p7; // filter reso
        param[8] = p8; // filter env
        param[9] = p9; // filter LFO
        param[10] = p10; // velocity
        param[11] = p11; // filter attack
        param[12] = p12; // filter decay
        param[13] = p13; // filter sustain
        param[14] = p14; // filter release
        param[15] = p15; // env attack
        param[16] = p16; // env decay
        param[17] = p17; // env sustain
        param[18] = p18; // env release
        param[19] = p19; // LFO rate
        param[20] = p20; // vibrato
        param[21] = p21; // noise
        param[22] = p22; // tuning
        param[23] = p23; // output level
        param[24] = p24; // polyphony
        param[25] = p25;
    }
    char name[40];
    float param[NUM_PARAMS];
};
