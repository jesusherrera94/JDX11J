/*
  ==============================================================================

    Synth.h
    Created: 18 Apr 2024 9:04:44pm
    Author:  Jesus Herrera

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "NoiseGenerator.h"

class Synth {
public:
    Synth();
    float noiseMix; // param from UI
    float envDecay; // param from UI
    float envSustain; // param fron UI
    float envRelease; // param from UI
    float envAttack; // param from UI
    float oscMix; // param from UI
    float detune; // combination of UI params
    float tune; // combination of UI params
    // float outputLevel; // main volume from UI
    float velocitySensitivity; // param from UI
    float vibrato; // parameter from UI
    float pwmDepth; // parameter from UI
    int glideMode; // parameter from UI
    float glideRate; // parameter from UI
    float glideBend; // parameter from UI
    float filterKeyTracking; // parameter combined from UI
    float filterQ; // parameter from UI
    float filterLFODepth; // parameter from UI
    float resonanceCtl;
    bool ignoreVelocity;
    float pressure;
    float filterCtl;
    float filterZip;
    float filterAttack, filterDecay, filterSustain, filterRelease; // params from UI
    float filterEnvDepth; // combination of paramos from UI
    uint8_t resoCC = 0x47;
    
    
    juce::LinearSmoothedValue<float> outputLevelSmoother; // similar to outputLevel
    
    static constexpr int MAX_VOICES = 8;
    int numVoices;
    const int LFO_MAX = 32;
    float lfoInc;
    int lfoStep;
    float lfo;
    
    // analog to prepareToPlate
    void allocateResources(double sampleRate, int samplesPerBlock);
    // analog to releaseResources
    void deallocateResources();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void reset();
    void startVoice(int v, int note, int velocity);
    int findFreeVoice() const;
    bool sustainPedalPressed;
    void controlChange(uint8_t data1, uint8_t data2);
    float volumeTrim;
    void restartMonoVoice(int note, int velocity);
    void shiftQueueNotes();
    int nextQueueNote();
    void updateLFO();
    
private:
    float sampleRate;
    // Voice voice;
    std::array<Voice, MAX_VOICES> voices;
    NoiseGenerator noiseGen;
    float pitchBend; // this is for midi controller bend note support!
    float modWheel;
    int lastNote;
    // methods
    void noteOn(int note, int velocity);
    void noteOff(int note);
    float calcPeriod(int v, int note) const;
    bool isPlayingLegatoStyle() const;
    inline void updatePeriod(Voice&  voice){
        voice.osc1.period = voice.period * pitchBend;
        voice.osc2.period = voice.osc1.period * detune; // * 0.994f ; <- add this with ui feature
    }
};

