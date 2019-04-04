/*
  ==============================================================================

    SynthAudioSource.h
    Created: 11 Feb 2019 12:26:54am
    Author:  Teresa

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SineWaveVoice.h"

//==============================================================================
class SynthAudioSource : public AudioSource
{
public:
	SynthAudioSource(MidiKeyboardState& keyState)
		: keyboardState(keyState)
	{
		for (auto i = 0; i < numVoices; ++i)
		{
			synthVoices.add(new SineWaveVoice(signalTable, numVoices));
			synth.addVoice(synthVoices.getLast());
		}
		synth.addSound(new SineWaveSound());
	}
	void setUsingSineWaveSound()
	{
		synth.clearSounds();
	}
	void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
	{
		synth.setCurrentPlaybackSampleRate(sampleRate);
		createWavetable(); // ***
	}
	void releaseResources() override {}
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		bufferToFill.clearActiveBufferRegion();
		MidiBuffer incomingMidi;
		midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
		keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
			bufferToFill.numSamples, true);
		synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
	}
	void passParameters(double parameter, int parID, int filterNum)
	{
		for (auto i = 0; i < numVoices; ++i)
		{
			synthVoices[i]->setFilterParameter(parameter, parID, filterNum);
		}
	}
	void passParameters(double parameter, int parID)
	{
		for (auto i = 0; i < numVoices; ++i)
		{
			synthVoices[i]->setEnvelopeParameter(parameter, parID);
		}
	}


	MidiMessageCollector* getMidiCollector()
	{
		return &midiCollector;
	}
	MidiMessageCollector midiCollector;
private:
	MidiKeyboardState& keyboardState;
	Synthesiser synth;
	OwnedArray<SineWaveVoice> synthVoices;

	int tableSize = 4096;
	int numVoices = 4;
	AudioSampleBuffer signalTable;
	void createWavetable()
	{
		signalTable.setSize(1, tableSize);
		auto* samples = signalTable.getWritePointer(0);
		for (auto i = 0; i < tableSize; ++i)
		{
			float ta = (float)i / (float)tableSize;
			auto sample = 2.0f * ( ta - std::floor(0.5f + ta) );
			samples[i] = sample;
		}
	}
};