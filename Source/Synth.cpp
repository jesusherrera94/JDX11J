/*
  ==============================================================================

    Synth.cpp
    Created: 18 Apr 2024 9:04:44pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#include "Synth.h"
#include "Utils.h"

static const float ANALOG = 1.0f;

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
    for (int v=0; v < MAX_VOICES; ++v) {
        voices[v].reset();
    }
    noiseGen.reset();
    pitchBend = 1.0f;
}

void Synth::render( float** outputBuffers, int sampleCount) {
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];
    // multiplied by pitchBend to handle bend from physical midi controller
//    voice.osc1.period = voice.period * pitchBend;
//    voice.osc2.period = voice.osc1.period * detune * 0.994f ;
    
    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = voices[v];
        if (voice.env.isActive()) {
            voice.osc1.period = voice.period * pitchBend;
            voice.osc2.period = voice.osc1.period * detune * 0.994f ;
        }
    }
    
    for (int sample = 0; sample < sampleCount; ++sample) {
        float noise = noiseGen.nextValue() * noiseMix;
        
        float outputLeft = 0.0f;
        float outputRight = 0.0f;
//        if (voice.env.isActive()) {
//            // old line to generate noise
//            // output = noise * (voice.velocity / 127.0f) * 0.5f;
//            // output = voice.render(noise); // adds noise to the signal....
//            float output = voice.render(noise);
//            outputLeft += output * voice.panLeft;
//            outputRight += output * voice.panRight;
//        }
        for (int v = 0; v < MAX_VOICES; ++v) {
            Voice& voice = voices[v];
            if (voice.env.isActive()) {
                float output = voice.render(noise);
                outputLeft += output * voice.panLeft;
                outputRight += output * voice.panRight;
            }
        }
        
        if (outputBufferRight != nullptr) {
            outputBufferLeft[sample] = outputLeft;
            outputBufferRight[sample] = outputRight;
        } else {
            outputBufferLeft[sample] = (outputLeft + outputRight) * 0.5f;
        }
    }
    
    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = voices[v];
        if (!voice.env.isActive()) {
            voice.env.reset();
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
        // pitch bend from physical midi controller
        case 0xE0:
            pitchBend = std::exp(-0.000014102f * float(data1 + 128 * data2 - 8192));
            break;
            
    }
}

void Synth::startVoice(int v, int note, int velocity) {
    float period = calcPeriod(v,note);
    Voice& voice = voices[v];
    voice.period = period;
    voice.note = note;
    voice.updatePanning();
    
    voice.osc1.amplitude = (velocity / 127.0f) * 0.5f;
    voice.osc2.amplitude = voice.osc1.amplitude * oscMix;
    
    Envelope& env = voice.env;
    
    env.attackMultiplier = envAttack;
    env.decayMultiplier = envDecay;
    env.sustainLevel = envSustain;
    env.releaseMultiplier = envRelease;
    env.attack();
}

void Synth::noteOn(int note, int velocity) {
    int v = 0; // index of the voice to use ( 0 = mono voice)
    if (numVoices > 1) { // polyphonic
        v = findFreeVoice();
    }
    startVoice(v, note, velocity);
}

void Synth::noteOff(int note) {
    for(int v = 0; v < MAX_VOICES; v++) {
        if (voices[v].note == note) {
            voices[v].release();
            voices[v].note = 0;
        }
    }
}

float Synth::calcPeriod(int v, int note) const {
    float period = tune * std::exp(-0.05776226505f * (float(note) + ANALOG * float(v)));
    while (period < 6.0f || (period * detune) < 6.0f) {
        period += period;
    }
    return period;
}

int Synth::findFreeVoice() const {
    int v = 0;
    float l = 100.0f; // louder than any envelope!
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (voices[i].env.level < l && !voices[i].env.isInAttack()) {
            l = voices[i].env.level;
            v = l;
        }
    }
    return v;
}
