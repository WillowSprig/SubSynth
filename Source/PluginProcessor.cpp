/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SyntSubAudioProcessor::SyntSubAudioProcessor() : synthAudioSource(keyboardState)
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SyntSubAudioProcessor::~SyntSubAudioProcessor()
{
}

//==============================================================================
const String SyntSubAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SyntSubAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SyntSubAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SyntSubAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SyntSubAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SyntSubAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SyntSubAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SyntSubAudioProcessor::setCurrentProgram (int index)
{
}

const String SyntSubAudioProcessor::getProgramName (int index)
{
    return {};
}

void SyntSubAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SyntSubAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	synthAudioSource.prepareToPlay(samplesPerBlock, sampleRate);
	synthAudioSource.midiCollector.reset(sampleRate);
	// Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SyntSubAudioProcessor::releaseResources()
{
	synthAudioSource.releaseResources();
	// When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SyntSubAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void SyntSubAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	MidiMessage m;
	int time;
	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
		synthAudioSource.midiCollector.addMessageToQueue(m);
	AudioSourceChannelInfo channelInfo = AudioSourceChannelInfo(buffer);
	synthAudioSource.getNextAudioBlock(channelInfo);
}

//==============================================================================
bool SyntSubAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SyntSubAudioProcessor::createEditor()
{
    return new SyntSubAudioProcessorEditor (*this);
}

//==============================================================================
void SyntSubAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SyntSubAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SyntSubAudioProcessor();
}
