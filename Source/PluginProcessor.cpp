/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JX11JAudioProcessor::JX11JAudioProcessor()
     : AudioProcessor (BusesProperties()
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       )

{
}

JX11JAudioProcessor::~JX11JAudioProcessor()
{
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JX11JAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JX11JAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JX11JAudioProcessor::getProgramName (int index)
{
    return {};
}

void JX11JAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
// Prepare to play is a function that is called before of everything and contains
// all the needed data to run in a daw host.
void JX11JAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.allocateResources(sampleRate, samplesPerBlock);
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
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

//==============================================================================
bool JX11JAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JX11JAudioProcessor::createEditor()
{
    return new JX11JAudioProcessorEditor (*this);
}

//==============================================================================
void JX11JAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JX11JAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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
    // process the message in the synth class
    synth.midiMessage(data0, data1, data2);
}

void JX11JAudioProcessor::render(juce::AudioBuffer<float>& buffer, int sampleCount, int bufferOffset) {
    float* outputBuffers[2] = { nullptr, nullptr };
    outputBuffers[0] = buffer.getWritePointer(0) + bufferOffset;
    if (getTotalNumOutputChannels() > 1) {
        outputBuffers[1] = buffer.getWritePointer(1) + bufferOffset;
    }
    
    synth.render(outputBuffers, sampleCount);
}

void JX11JAudioProcessor::reset() {
    synth.reset();
}
