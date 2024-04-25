/*
  ==============================================================================

    Synth.cpp
    Created: 18 Apr 2024 9:04:44pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#include "Synth.h"
#include "Utils.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
}

void Synth::allocateResources(double sampleRate_, int /*samplesPerBlock*/) {
    sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateResources(){
    // do nothing
}

void Synth::reset() {
    voice.reset();
    noiseGen.reset();
}

void Synth::render( float** outputBuffers, int sampleCount) {
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];
    
    for (int sample = 0; sample < sampleCount; ++sample) {
        float noise = noiseGen.nextValue();
        
        float output = 0.0f;
        if (voice.note > 0) {
            // old line to generate noise
            // output = noise * (voice.velocity / 127.0f) * 0.5f;
            output = voice.render();
        }
        outputBufferLeft[sample] = output;
        if (outputBufferRight != nullptr) {
            outputBufferRight[sample] = output;
        }
    }
    protectYourEars(outputBufferLeft, sampleCount);
    protectYourEars(outputBufferRight, sampleCount);
    
}

void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2) {
    uint8_t channel = data0 & 0x0F; // get the channel
    switch (data0 & 0xF0) {
        case 0x80:
            noteOff(data1 & 0x7F);
            break;
        case 0x90:
            {
                uint8_t note = data1 & 0x7F;
                uint8_t velo = data2 & 0x7F;
                if (velo > 0) {
                    noteOn(note, velo);
                } else {
                    noteOff(note);
                }
                break;
            }
    }
}

void Synth::noteOn(int note, int velocity) {
    // old implementation with compkete formula
//    voice.note = note;
//    voice.osc.amplitude = (velocity / 127.0f) * 0.5f;
//    voice.osc.freq = 261.63f;
//    voice.osc.sampleRate = sampleRate;
//    voice.osc.phaseOffset = 0.0f;
//    voice.osc.reset();
    voice.note = note;
    // fixed freq
    // float freq = 25000.0f;
    float freq = 440.0f * std::exp2(float(note - 69 ) / 12.0f);
    voice.osc.amplitude = (velocity / 127.0f) * 0.5f;
    voice.osc.inc = freq / sampleRate;
    voice.osc.reset();
}

void Synth::noteOff(int note) {
    if (voice.note == note) {
        voice.note = 0;
    }
}
