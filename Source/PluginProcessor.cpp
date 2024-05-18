/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils.h"

//==============================================================================
JX11JAudioProcessor::JX11JAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif

{
    // all the parameters are linked here!!!!!
    castParameter(apvts, ParameterID::oscMix, oscMixParam);
    castParameter(apvts, ParameterID::oscTune, oscTuneParam);
    castParameter(apvts, ParameterID::oscFine, oscFineParam);
    castParameter(apvts, ParameterID::glideMode, glideModeParam);
    castParameter(apvts, ParameterID::glideRate, glideRateParam);
    castParameter(apvts, ParameterID::glideBend, glideBendParam);
    castParameter(apvts, ParameterID::filterFreq, filterFreqParam);
    castParameter(apvts, ParameterID::filterReso, filterResoParam);
    castParameter(apvts, ParameterID::filterEnv, filterEnvParam);
    castParameter(apvts, ParameterID::filterLFO, filterLFOParam);
    castParameter(apvts, ParameterID::filterVelocity, filterVelocityParam);
    castParameter(apvts, ParameterID::filterAttack, filterAttackParam);
    castParameter(apvts, ParameterID::filterDecay, filterDecayParam);
    castParameter(apvts, ParameterID::filterSustain, filterSustainParam);
    castParameter(apvts, ParameterID::filterRelease, filterReleaseParam);
    castParameter(apvts, ParameterID::envAttack, envAttackParam);
    castParameter(apvts, ParameterID::envDecay, envDecayParam);
    castParameter(apvts, ParameterID::envSustain, envSustainParam);
    castParameter(apvts, ParameterID::envRelease, envReleaseParam);
    castParameter(apvts, ParameterID::lfoRate, lfoRateParam);
    castParameter(apvts, ParameterID::vibrato, vibratoParam);
    castParameter(apvts, ParameterID::noise, noiseParam);
    castParameter(apvts, ParameterID::octave, octaveParam);
    castParameter(apvts, ParameterID::tuning, tuningParam);
    castParameter(apvts, ParameterID::outputLevel, outputLevelParam);
    castParameter(apvts, ParameterID::polyMode, polyModeParam);
    apvts.state.addListener(this);
    
    // ========= for creating presets/programs
    createPrograms();
    setCurrentProgram(0);
}

JX11JAudioProcessor::~JX11JAudioProcessor()
{
    apvts.state.removeListener(this); //remove lister when processor is destroyed!
}

//==============================================================================
const juce::String JX11JAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JX11JAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JX11JAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JX11JAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JX11JAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JX11JAudioProcessor::getNumPrograms()
{
    return int(presets.size());  // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JX11JAudioProcessor::getCurrentProgram()
{
    return currentProgram;
}

void JX11JAudioProcessor::setCurrentProgram (int index)
{
    currentProgram = index;
    juce::RangedAudioParameter *params[NUM_PARAMS] = {
        oscMixParam,
        oscTuneParam,
        oscFineParam,
        glideModeParam,
        glideRateParam,
        glideBendParam,
        filterFreqParam,
        filterResoParam,
        filterEnvParam,
        filterLFOParam,
        filterVelocityParam,
        filterAttackParam,
        filterDecayParam,
        filterSustainParam,
        filterReleaseParam,
        envAttackParam,
        envDecayParam,
        envSustainParam,
        envReleaseParam,
        lfoRateParam,
        vibratoParam,
        noiseParam,
        octaveParam,
        tuningParam,
        outputLevelParam,
        polyModeParam,
    };
    
    const Preset& preset = presets[index];
    
    for (int i = 0; i < NUM_PARAMS; ++i) {
        params[i] ->setValueNotifyingHost(params[i]->convertTo0to1(preset.param[i]));
    }
    reset();
}

const juce::String JX11JAudioProcessor::getProgramName (int index)
{
    return { presets[index].name };
}

void JX11JAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    // not implemented
}

//==============================================================================
// Prepare to play is a function that is called before of everything and contains
// all the needed data to run in a daw host.
void JX11JAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.allocateResources(sampleRate, samplesPerBlock);
    parametersChanged.store(true);
    reset();
}

void JX11JAudioProcessor::releaseResources()
{
    synth.deallocateResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JX11JAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    #if JucePlugin_IsMidiEffect
      juce::ignoreUnused (layouts);
      return true;
    #else
      // This is the place where you check if the layout is supported.
      // In this template code we only support mono or stereo.
      // Some plugin hosts, such as certain GarageBand versions, will only
      // load plugins that support stereo bus layouts.
      if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
       && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
          return false;

      // This checks if the input layout matches the output layout
     #if ! JucePlugin_IsSynth
      if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
          return false;
     #endif

      return true;
    #endif
}
#endif

// The first parameter is the geneerated audio, and the second param is the recieved message
// midiMessages use to be empty most of the time, but something change we need to handle it
void JX11JAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals; // This line converts the super small numbers to zero basically
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // to update parameters from UI
    bool expected = true;
    if ( isNonRealtime() || parametersChanged.compare_exchange_strong(expected, false) ) { // if it is true, returned to false in this instruction
        update();
    }
    
    // process messages
    splitBufferByEvents(buffer, midiMessages);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //    {
    //        auto* channelData = buffer.getWritePointer (channel);
    //
    //        // ..do something to the data...
    //    }
}

// =============================== GET UPDATES FROM UI PARAMETERS ==============

void JX11JAudioProcessor::update() {
    float noiseMix = noiseParam->get() / 100.0f;
    noiseMix *= noiseMix;
    synth.noiseMix = noiseMix * 0.06f;
}

//==============================================================================
bool JX11JAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

//To display UI this method is used!!!!!
juce::AudioProcessorEditor* JX11JAudioProcessor::createEditor()
{
    // original juce code
    // return new JX11JAudioProcessorEditor (*this);
    // code implemented for creating my personalized UI
    auto editor = new juce::GenericAudioProcessorEditor(*this);
    editor->setSize(500,1050);
    return editor;
}

//==============================================================================
void JX11JAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
    DBG(apvts.copyState().toXmlString());
}

void JX11JAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
        parametersChanged.store(true);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JX11JAudioProcessor();
}

// Personalized code!!!!
void JX11JAudioProcessor::splitBufferByEvents(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    int bufferOffset = 0;
    for (const auto metadata: midiMessages) {
        // Render the audio that happens before this event (if any).
        int samplesThisSegment = metadata.samplePosition - bufferOffset;
        if (samplesThisSegment) {
            render(buffer, samplesThisSegment, bufferOffset);
            bufferOffset += samplesThisSegment;
        }
        
        // Handle the event. Ignore MIDI messages such as sysex
        if (metadata.numBytes <= 3) {
            uint8_t data1 = (metadata.numBytes >= 2) ? metadata.data[1] : 0;
            uint8_t data2 = (metadata.numBytes ==3) ? metadata.data[2] : 0;
            handleMIDI(metadata.data[0], data1, data2);
        }
    }
    
    // Render the audio after the last MIDI event, If there were
    // MIDI events at all, this renders the entire buffer.
    int samplesLastSegment = buffer.getNumSamples() - bufferOffset;
    if (samplesLastSegment > 0) {
        render(buffer, samplesLastSegment, bufferOffset);
    }
    midiMessages.clear();
}

void JX11JAudioProcessor::handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2) {
    // Program change
    if ((data0 & 0xF0) == 0xC0) {
        if (data1 < presets.size()) {
            setCurrentProgram(data1); // this only can be tested using midi keyboards with program change
            // TODO: test it later!!!!!             
        }
    }
    // process the message in the synth class
    synth.midiMessage(data0, data1, data2);
}

void JX11JAudioProcessor::render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset) {
    float* outputBuffers[2] = { nullptr, nullptr };
    outputBuffers[0] = buffer.getWritePointer(0) + bufferOffset; // only left channel is configured to sound in this line
    /**
     // to configure the right channel the line would look like
     */
    if (getTotalNumOutputChannels() > 1) {
        outputBuffers[1] = buffer.getWritePointer(1) + bufferOffset;
    }
    
    synth.render(outputBuffers, sampleCount);
}

void JX11JAudioProcessor::reset() {
    synth.reset();
}

// For UI initialization and controls implementation

juce::AudioProcessorValueTreeState::ParameterLayout
    JX11JAudioProcessor::createParameterLayout() {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        // Implement parameters
        // Polyphony
        layout.add(std::make_unique<juce::AudioParameterChoice>(
                                                                ParameterID::polyMode,
                                                                "Polyphony",
                                                                juce::StringArray{"Mono", "Poly"},
                                                                1));
        // Osc Tune
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::oscTune,
                                                               "Osc Tune",
                                                               juce::NormalisableRange<float>(-24.0f, 24.0f, 1.0f),
                                                               -12.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("semi")));
        // Osc Fine
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::oscFine,
                                                               "Osc Fine",
                                                               juce::NormalisableRange<float>(-50.0f, 50.0f, 0.1f, 0.3f, true),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("cent")));
        
        // PERSONALIZED PARAMETER!!!!!!!
        auto oscMixStringFromValue = [](float value, int) {
            char s[16] = { 0 };
            snprintf(s, 16, "%4.0f:%2.0f", 100.0 - 0.5f * value, 0.5f * value);
            return juce::String(s);
        };
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::oscMix,
                                                               "Osc Mix",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes()
                                                               .withLabel("%")
                                                               .withStringFromValueFunction(oscMixStringFromValue)));
        // Glide Mode
        layout.add(std::make_unique<juce::AudioParameterChoice>(
                                                               ParameterID::glideMode,
                                                               "Glide Mode",
                                                                juce::StringArray {"Off", "Legato", "Always"},
                                                            0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::glideRate,
                                                               "Glide Rate",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               35.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::glideBend,
                                                               "Glide Bend",
                                                               juce::NormalisableRange<float>(-36.0f, 36.0f, 0.01f, 0.4f, true),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("semi")));
        // Filter
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterFreq,
                                                               "Filter Freq",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                               100.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        // Resonance
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterReso,
                                                               "Filter Reso",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               15.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterEnv,
                                                               "Filter Env",
                                                               juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
                                                               50.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterLFO,
                                                               "Filter LFO",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        // lambda for velocity
        auto filterVelocityStringFromValue = [](float value, int) {
            if (value < -90.0f) {
                return juce::String("OFF");
            } else {
                return juce::String(value);
            }
        };
        
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterVelocity,
                                                               "Velocity",
                                                               juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes()
                                                               .withLabel("%")
                                                               .withStringFromValueFunction(filterVelocityStringFromValue)));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterAttack,
                                                               "Filter Attack",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterDecay,
                                                               "Filter Decay",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               30.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterSustain,
                                                               "Filter Sustain",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::filterRelease,
                                                               "Filter Release",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               25.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        // Envelope parameters!!!!!
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::envAttack,
                                                               "Env Attack",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::envDecay,
                                                               "Env Decay",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               50.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::envSustain,
                                                               "Env Sustain",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               100.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::envRelease,
                                                               "Env Release",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               30.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        // lambda for LFO rate for envelope
        auto lfoRateStringFromValue = [](float value, int) {
            float lfohz = std::exp(7.0f * value - 4.0f);
            return juce::String(lfohz, 3);
        };
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::lfoRate,
                                                               "LFO Rate",
                                                               juce::NormalisableRange<float>(),
                                                               0.81f,
                                                               juce::AudioParameterFloatAttributes()
                                                               .withLabel("Hz")
                                                               .withStringFromValueFunction(lfoRateStringFromValue)));
        // vibrato lambda implementation
        auto vibratoStringFromValue = [](float value, int) {
            if (value < 0.0f) {
                return "PWM " + juce::String(-value, 1);
            } else {
                return juce::String(value, 1);
            }
        };
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::vibrato,
                                                               "Vibrato",
                                                               juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes()
                                                               .withLabel("%")
                                                               .withStringFromValueFunction(vibratoStringFromValue)));
        // noise
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::noise,
                                                               "Noise",
                                                               juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("%")));
        // octave
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::octave,
                                                               "Octave",
                                                               juce::NormalisableRange<float>(-2.0f, 2.0f, 1.0f),
                                                               0.0f
                                                               ));
        // tuning
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::tuning,
                                                               "Tuning",
                                                               juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("cent")));
        // Output level
        layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                               ParameterID::outputLevel,
                                                               "Output Level",
                                                               juce::NormalisableRange<float>(-24.0f, 6.0f, 0.1f),
                                                               0.0f,
                                                               juce::AudioParameterFloatAttributes().withLabel("dB")));
        return layout;
}

// ============== for managing preset/programs ===============


void JX11JAudioProcessor::createPrograms()
{
    presets.emplace_back("Init", 0.00f, -12.00f, 0.00f, 0.00f, 35.00f, 0.00f, 100.00f, 15.00f, 50.00f, 0.00f, 0.00f, 0.00f, 30.00f, 0.00f, 25.00f, 0.00f, 50.00f, 100.00f, 30.00f, 0.81f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("5th Sweep Pad", 100.00f, -7.00f, -6.30f, 1.00f, 32.00f, 0.00f, 90.00f, 60.00f, -76.00f, 0.00f, 0.00f, 90.00f, 89.00f, 90.00f, 73.00f, 0.00f, 50.00f, 100.00f, 71.00f, 0.81f, 30.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Echo Pad [SA]", 88.00f, 0.00f, 0.00f, 0.00f, 49.00f, 0.00f, 46.00f, 76.00f, 38.00f, 10.00f, 38.00f, 100.00f, 86.00f, 76.00f, 57.00f, 30.00f, 80.00f, 68.00f, 66.00f, 0.79f, -74.00f, 25.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Space Chimes [SA]", 88.00f, 0.00f, 0.00f, 0.00f, 49.00f, 0.00f, 49.00f, 82.00f, 32.00f, 8.00f, 78.00f, 85.00f, 69.00f, 76.00f, 47.00f, 12.00f, 22.00f, 55.00f, 66.00f, 0.89f, -32.00f, 0.00f, 2.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Solid Backing", 100.00f, -12.00f, -18.70f, 0.00f, 35.00f, 0.00f, 30.00f, 25.00f, 40.00f, 0.00f, 26.00f, 0.00f, 35.00f, 0.00f, 25.00f, 0.00f, 50.00f, 100.00f, 30.00f, 0.81f, 0.00f, 50.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Velocity Backing [SA]", 41.00f, 0.00f, 9.70f, 0.00f, 8.00f, -1.68f, 49.00f, 1.00f, -32.00f, 0.00f, 86.00f, 61.00f, 87.00f, 100.00f, 93.00f, 11.00f, 48.00f, 98.00f, 32.00f, 0.81f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Rubber Backing [ZF]", 29.00f, 12.00f, -5.60f, 0.00f, 18.00f, 5.06f, 35.00f, 15.00f, 54.00f, 14.00f, 8.00f, 0.00f, 42.00f, 13.00f, 21.00f, 0.00f, 56.00f, 0.00f, 32.00f, 0.20f, 16.00f, 22.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("808 State Lead", 100.00f, 7.00f, -7.10f, 2.00f, 34.00f, 12.35f, 65.00f, 63.00f, 50.00f, 16.00f, 0.00f, 0.00f, 30.00f, 0.00f, 25.00f, 17.00f, 50.00f, 100.00f, 3.00f, 0.81f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Mono Glide", 0.00f, -12.00f, 0.00f, 2.00f, 46.00f, 0.00f, 51.00f, 0.00f, 0.00f, 0.00f, -100.00f, 0.00f, 30.00f, 0.00f, 25.00f, 37.00f, 50.00f, 100.00f, 38.00f, 0.81f, 24.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Detuned Techno Lead", 84.00f, 0.00f, -17.20f, 2.00f, 41.00f, -0.15f, 54.00f, 1.00f, 16.00f, 21.00f, 34.00f, 0.00f, 9.00f, 100.00f, 25.00f, 20.00f, 85.00f, 100.00f, 30.00f, 0.83f, -82.00f, 40.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Hard Lead [SA]", 71.00f, 12.00f, 0.00f, 0.00f, 24.00f, 36.00f, 56.00f, 52.00f, 38.00f, 19.00f, 40.00f, 100.00f, 14.00f, 65.00f, 95.00f, 7.00f, 91.00f, 100.00f, 15.00f, 0.84f, -34.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Bubble", 0.00f, -12.00f, -0.20f, 0.00f, 71.00f, -0.00f, 23.00f, 77.00f, 60.00f, 32.00f, 26.00f, 40.00f, 18.00f, 66.00f, 14.00f, 0.00f, 38.00f, 65.00f, 16.00f, 0.48f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Monosynth", 62.00f, -12.00f, 0.00f, 1.00f, 35.00f, 0.02f, 64.00f, 39.00f, 2.00f, 65.00f, -100.00f, 7.00f, 52.00f, 24.00f, 84.00f, 13.00f, 30.00f, 76.00f, 21.00f, 0.58f, -40.00f, 0.00f, -1.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Moogcury Lite", 81.00f, 24.00f, -9.80f, 1.00f, 15.00f, -0.97f, 39.00f, 17.00f, 38.00f, 40.00f, 24.00f, 0.00f, 47.00f, 19.00f, 37.00f, 0.00f, 50.00f, 20.00f, 33.00f, 0.38f, 6.00f, 0.00f, -2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Gangsta Whine", 0.00f, 0.00f, 0.00f, 2.00f, 44.00f, 0.00f, 41.00f, 46.00f, 0.00f, 0.00f, -100.00f, 0.00f, 0.00f, 100.00f, 25.00f, 15.00f, 50.00f, 100.00f, 32.00f, 0.81f, -2.00f, 0.00f, 2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Higher Synth [ZF]", 48.00f, 0.00f, -8.80f, 0.00f, 0.00f, 0.00f, 50.00f, 47.00f, 46.00f, 30.00f, 60.00f, 0.00f, 10.00f, 0.00f, 7.00f, 0.00f, 42.00f, 0.00f, 22.00f, 0.21f, 18.00f, 16.00f, 2.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("303 Saw Bass", 0.00f, 0.00f, 0.00f, 1.00f, 49.00f, 0.00f, 55.00f, 75.00f, 38.00f, 35.00f, 0.00f, 0.00f, 56.00f, 0.00f, 56.00f, 0.00f, 80.00f, 100.00f, 24.00f, 0.26f, -2.00f, 0.00f, -2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("303 Square Bass", 75.00f, 0.00f, 0.00f, 1.00f, 49.00f, 0.00f, 55.00f, 75.00f, 38.00f, 35.00f, 0.00f, 14.00f, 49.00f, 0.00f, 39.00f, 0.00f, 80.00f, 100.00f, 24.00f, 0.26f, -2.00f, 0.00f, -2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Analog Bass", 100.00f, -12.00f, -10.90f, 1.00f, 19.00f, 0.00f, 30.00f, 51.00f, 70.00f, 9.00f, -100.00f, 0.00f, 88.00f, 0.00f, 21.00f, 0.00f, 50.00f, 100.00f, 46.00f, 0.81f, 0.00f, 0.00f, -1.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Analog Bass 2", 100.00f, -12.00f, -10.90f, 0.00f, 19.00f, 13.44f, 48.00f, 43.00f, 88.00f, 0.00f, 60.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 61.00f, 100.00f, 32.00f, 0.81f, 0.00f, 0.00f, -1.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Low Pulses", 97.00f, -12.00f, -3.30f, 0.00f, 35.00f, 0.00f, 80.00f, 40.00f, 4.00f, 0.00f, 0.00f, 0.00f, 77.00f, 0.00f, 25.00f, 0.00f, 50.00f, 100.00f, 30.00f, 0.81f, -68.00f, 0.00f, -2.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Sine Infra-Bass", 0.00f, -12.00f, 0.00f, 0.00f, 35.00f, 0.00f, 33.00f, 76.00f, 6.00f, 0.00f, 0.00f, 0.00f, 30.00f, 0.00f, 25.00f, 0.00f, 55.00f, 25.00f, 30.00f, 0.81f, 4.00f, 0.00f, -2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Wobble Bass [SA]", 100.00f, -12.00f, -8.80f, 0.00f, 82.00f, 0.21f, 72.00f, 47.00f, -32.00f, 34.00f, 64.00f, 20.00f, 69.00f, 100.00f, 15.00f, 9.00f, 50.00f, 100.00f, 7.00f, 0.81f, -8.00f, 0.00f, -1.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Squelch Bass", 100.00f, -12.00f, -8.80f, 0.00f, 35.00f, 0.00f, 67.00f, 70.00f, -48.00f, 0.00f, 0.00f, 48.00f, 69.00f, 100.00f, 15.00f, 0.00f, 50.00f, 100.00f, 7.00f, 0.81f, -8.00f, 0.00f, -1.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Rubber Bass [ZF]", 49.00f, -12.00f, 1.60f, 1.00f, 35.00f, 0.00f, 36.00f, 15.00f, 50.00f, 20.00f, 0.00f, 0.00f, 38.00f, 0.00f, 25.00f, 0.00f, 60.00f, 100.00f, 22.00f, 0.19f, 0.00f, 0.00f, -2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Soft Pick Bass", 37.00f, 0.00f, 7.80f, 0.00f, 22.00f, 0.00f, 33.00f, 47.00f, 42.00f, 16.00f, 18.00f, 0.00f, 0.00f, 0.00f, 25.00f, 4.00f, 58.00f, 0.00f, 22.00f, 0.15f, -12.00f, 33.00f, -2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Fretless Bass", 50.00f, 0.00f, -14.40f, 1.00f, 34.00f, 0.00f, 51.00f, 0.00f, 16.00f, 0.00f, 34.00f, 0.00f, 9.00f, 0.00f, 25.00f, 20.00f, 85.00f, 0.00f, 30.00f, 0.81f, 40.00f, 0.00f, -2.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Whistler", 23.00f, 0.00f, -0.70f, 0.00f, 35.00f, 0.00f, 33.00f, 100.00f, 0.00f, 0.00f, 0.00f, 0.00f, 29.00f, 0.00f, 25.00f, 68.00f, 39.00f, 58.00f, 36.00f, 0.81f, 28.00f, 38.00f, 2.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Very Soft Pad", 39.00f, 0.00f, -4.90f, 2.00f, 12.00f, 0.00f, 35.00f, 78.00f, 0.00f, 0.00f, 0.00f, 0.00f, 30.00f, 0.00f, 25.00f, 35.00f, 50.00f, 80.00f, 70.00f, 0.81f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Pizzicato", 0.00f, -12.00f, 0.00f, 0.00f, 35.00f, 0.00f, 23.00f, 20.00f, 50.00f, 0.00f, 0.00f, 0.00f, 22.00f, 0.00f, 25.00f, 0.00f, 47.00f, 0.00f, 30.00f, 0.81f, 0.00f, 80.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Synth Strings", 100.00f, 0.00f, -7.10f, 0.00f, 0.00f, -0.97f, 42.00f, 26.00f, 50.00f, 14.00f, 38.00f, 0.00f, 67.00f, 55.00f, 97.00f, 82.00f, 70.00f, 100.00f, 42.00f, 0.84f, 34.00f, 30.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Synth Strings 2", 75.00f, 0.00f, -3.80f, 0.00f, 49.00f, 0.00f, 55.00f, 16.00f, 38.00f, 8.00f, -60.00f, 76.00f, 29.00f, 76.00f, 100.00f, 46.00f, 80.00f, 100.00f, 39.00f, 0.79f, -46.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Leslie Organ", 0.00f, 0.00f, 0.00f, 0.00f, 13.00f, -0.38f, 38.00f, 74.00f, 8.00f, 20.00f, -100.00f, 0.00f, 55.00f, 52.00f, 31.00f, 0.00f, 17.00f, 73.00f, 28.00f, 0.87f, -52.00f, 0.00f, -1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Click Organ", 50.00f, 12.00f, 0.00f, 0.00f, 35.00f, 0.00f, 44.00f, 50.00f, 30.00f, 16.00f, -100.00f, 0.00f, 0.00f, 18.00f, 0.00f, 0.00f, 75.00f, 80.00f, 0.00f, 0.81f, -2.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Hard Organ", 89.00f, 19.00f, -0.90f, 0.00f, 35.00f, 0.00f, 51.00f, 62.00f, 8.00f, 0.00f, -100.00f, 0.00f, 37.00f, 0.00f, 100.00f, 4.00f, 8.00f, 72.00f, 4.00f, 0.77f, -2.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Bass Clarinet", 100.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 51.00f, 10.00f, 0.00f, 11.00f, 0.00f, 0.00f, 0.00f, 0.00f, 25.00f, 35.00f, 65.00f, 65.00f, 32.00f, 0.79f, -2.00f, 20.00f, -1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Trumpet", 0.00f, 0.00f, 0.00f, 1.00f, 6.00f, 0.00f, 57.00f, 0.00f, -36.00f, 15.00f, 0.00f, 21.00f, 15.00f, 0.00f, 25.00f, 24.00f, 60.00f, 80.00f, 10.00f, 0.75f, 10.00f, 25.00f, 1.00f, 0.00f, 0.00f, 0.00f);
    presets.emplace_back("Soft Horn", 12.00f, 19.00f, 1.90f, 0.00f, 35.00f, 0.00f, 50.00f, 21.00f, -42.00f, 12.00f, 20.00f, 0.00f, 35.00f, 36.00f, 25.00f, 8.00f, 50.00f, 100.00f, 27.00f, 0.83f, 2.00f, 10.00f, -1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Brass Section", 43.00f, 12.00f, -7.90f, 0.00f, 28.00f, -0.79f, 50.00f, 0.00f, 18.00f, 0.00f, 0.00f, 24.00f, 16.00f, 91.00f, 8.00f, 17.00f, 50.00f, 80.00f, 45.00f, 0.81f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Synth Brass", 40.00f, 0.00f, -6.30f, 0.00f, 30.00f, -3.07f, 39.00f, 15.00f, 50.00f, 0.00f, 0.00f, 39.00f, 30.00f, 82.00f, 25.00f, 33.00f, 74.00f, 76.00f, 41.00f, 0.81f, -6.00f, 23.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Detuned Syn Brass [ZF]", 68.00f, 0.00f, 31.80f, 0.00f, 31.00f, 0.50f, 26.00f, 7.00f, 70.00f, 0.00f, 32.00f, 0.00f, 83.00f, 0.00f, 5.00f, 0.00f, 75.00f, 54.00f, 32.00f, 0.76f, -26.00f, 29.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Power PWM", 100.00f, -12.00f, -8.80f, 0.00f, 35.00f, 0.00f, 82.00f, 13.00f, 50.00f, 0.00f, -100.00f, 24.00f, 30.00f, 88.00f, 34.00f, 0.00f, 50.00f, 100.00f, 48.00f, 0.71f, -26.00f, 0.00f, -1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Water Velocity [SA]", 76.00f, 0.00f, -1.40f, 0.00f, 49.00f, 0.00f, 87.00f, 67.00f, 100.00f, 32.00f, -82.00f, 95.00f, 56.00f, 72.00f, 100.00f, 4.00f, 76.00f, 11.00f, 46.00f, 0.88f, 44.00f, 0.00f, -1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Ghost [SA]", 75.00f, 0.00f, -7.10f, 2.00f, 16.00f, -0.00f, 38.00f, 58.00f, 50.00f, 16.00f, 62.00f, 0.00f, 30.00f, 40.00f, 31.00f, 37.00f, 50.00f, 100.00f, 54.00f, 0.85f, 66.00f, 43.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Soft E.Piano", 31.00f, 0.00f, -0.20f, 0.00f, 35.00f, 0.00f, 34.00f, 26.00f, 6.00f, 0.00f, 26.00f, 0.00f, 22.00f, 0.00f, 39.00f, 0.00f, 80.00f, 0.00f, 44.00f, 0.81f, 2.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Thumb Piano", 72.00f, 15.00f, 50.00f, 0.00f, 35.00f, 0.00f, 37.00f, 47.00f, 8.00f, 0.00f, 0.00f, 0.00f, 45.00f, 0.00f, 39.00f, 0.00f, 39.00f, 0.00f, 48.00f, 0.81f, 20.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Steel Drums [ZF]", 81.00f, 12.00f, -12.00f, 0.00f, 18.00f, 2.30f, 40.00f, 30.00f, 8.00f, 17.00f, -20.00f, 0.00f, 42.00f, 23.00f, 47.00f, 12.00f, 48.00f, 0.00f, 49.00f, 0.53f, -28.00f, 34.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Car Horn", 57.00f, -1.00f, -2.80f, 0.00f, 35.00f, 0.00f, 46.00f, 0.00f, 36.00f, 0.00f, 0.00f, 46.00f, 30.00f, 100.00f, 23.00f, 30.00f, 50.00f, 100.00f, 31.00f, 1.00f, -24.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Helicopter", 0.00f, -12.00f, 0.00f, 0.00f, 35.00f, 0.00f, 8.00f, 36.00f, 38.00f, 100.00f, 0.00f, 100.00f, 100.00f, 0.00f, 100.00f, 96.00f, 50.00f, 100.00f, 92.00f, 0.97f, 0.00f, 100.00f, -2.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Arctic Wind", 0.00f, -12.00f, 0.00f, 0.00f, 35.00f, 0.00f, 16.00f, 85.00f, 0.00f, 28.00f, 0.00f, 37.00f, 30.00f, 0.00f, 25.00f, 89.00f, 50.00f, 100.00f, 89.00f, 0.24f, 0.00f, 100.00f, 2.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Thip", 100.00f, -7.00f, 0.00f, 0.00f, 35.00f, 0.00f, 0.00f, 100.00f, 94.00f, 0.00f, 0.00f, 2.00f, 20.00f, 0.00f, 20.00f, 0.00f, 46.00f, 0.00f, 30.00f, 0.81f, 0.00f, 78.00f, 0.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Synth Tom", 0.00f, -12.00f, 0.00f, 0.00f, 76.00f, 24.53f, 30.00f, 33.00f, 52.00f, 0.00f, 36.00f, 0.00f, 59.00f, 0.00f, 59.00f, 10.00f, 50.00f, 0.00f, 50.00f, 0.81f, 0.00f, 70.00f, -2.00f, 0.00f, 0.00f, 1.00f);
    presets.emplace_back("Squelchy Frog", 50.00f, -5.00f, -7.90f, 2.00f, 77.00f, -36.00f, 40.00f, 65.00f, 90.00f, 0.00f, 0.00f, 33.00f, 50.00f, 0.00f, 25.00f, 0.00f, 70.00f, 65.00f, 18.00f, 0.32f, 100.00f, 0.00f, -2.00f, 0.00f, 0.00f, 1.00f);
}
