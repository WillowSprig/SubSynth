/*
  ==============================================================================

    SineWaveVoice.h
    Created: 11 Feb 2019 12:28:19am
    Author:  Teresa

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
struct SineWaveSound : public SynthesiserSound
{
	SineWaveSound() {}
	bool appliesToNote(int) override { return true; }
	bool appliesToChannel(int) override { return true; }
};

//==============================================================================
class SineWaveVoice : public SynthesiserVoice
{
public:
	// ***
	SineWaveVoice(const AudioSampleBuffer& wavetableToUse, unsigned int voices) : wavetable(wavetableToUse)
	{
		globalLevel = 0.5f / (float)voices;
		
		//jassert(wavetable.getNumChannels() == 1);

		for (int f = 0; f < numOfFilters; f++)
		{
			filterL[f].reset();
			filterR[f].reset();
		}
	}
	void setFrequency(double frequency, double sampleRate)
	{
		auto tableSizeOverSampleRate = wavetable.getNumSamples() / sampleRate;
		tableDelta = (float)(frequency * tableSizeOverSampleRate);
		currFrequency = frequency;

		for (int i = 0; i < numOfFilters; i++)
		{
			if ((currFrequency * filterFreqRatio[i]) < (sampleRate * 0.5))
				filterFreq[i] = currFrequency * filterFreqRatio[i];
			else
				filterFreq[i] = sampleRate * 0.5;
		}
	}
	void setFilterParameter(double parameter, int parID, int filterNum)
	{
		switch (parID)
		{
		case 0: //frequency of filter
		{
			filterFreqRatio[filterNum] = parameter;
			break;
		}
		case 1: // filter quality
		{
			filterQ[filterNum] = parameter;
			break;
		}
		case 2: //filter gain
		{
			parameter = pow(10.0, parameter / 20.0); //calculate gain from dB to ratio
			filterGain[filterNum] = (float)parameter;
		}
		}
	}
	void setEnvelopeParameter(double parameter, int parID)
	{
		float envDif = 0.0f;
		switch (parID)
		{
		case 0:
		{
			envDif = 1.0f * 1000.0f / (float)(getSampleRate() * parameter);
			break;
		}
		case 1:
		{
			envDif = powf(sustainLvl, 1000.0f / (float)(parameter * getSampleRate()));
			//envDif = (sustainLvl - 1.0f) * 1000.0f / (float)(getSampleRate() * parameter);
			break;
		}
		case 3:
		{
			envDif = powf((noteOffLvl / sustainLvl), 1000.0f / (float)(parameter * getSampleRate()) );
			break;
		}
		}
		envelopeChange[parID] = envDif;
		//DBG(envDif);		
	}
	void designFilters()
	{
		//'harmonic' filter
		IIRCoefficients coef = IIRCoefficients::makeLowPass(getSampleRate(), filterFreq[0], filterQ[0]);
		filterL[0].setCoefficients(coef);
		filterR[0].setCoefficients(coef);


		//formant filters
		for (int f = 1; f < numOfFilters; f++)
		{
			coef = IIRCoefficients::makePeakFilter(getSampleRate(),
				filterFreq[f],
				filterQ[f],
				filterGain[f]);
			filterL[f].setCoefficients(coef);
			filterR[f].setCoefficients(coef);
		}
	}
	forcedinline float getNextSample() noexcept
	{
		auto tableSize = wavetable.getNumSamples();
		auto index0 = (unsigned int)currentIndex;
		auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;
		auto frac = currentIndex - (float)index0;
		auto* table = wavetable.getReadPointer(0);
		auto value0 = table[index0];
		auto value1 = table[index1];
		auto currentSample = value0 + frac * (value1 - value0);
		if ((currentIndex += tableDelta) > tableSize)
			currentIndex -= tableSize;
		return currentSample;
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SineWaveSound*> (sound) != nullptr;
	}
	void startNote(int midiNoteNumber, float velocity,
		SynthesiserSound*, int /*currentPitchWheelPosition*/) override
	{
		currentIndex = 0.0;
		level = velocity * globalLevel;
		currEnvState = Attack;
		currEnvelope = 0.0f;

		setFrequency(MidiMessage::getMidiNoteInHertz(midiNoteNumber), getSampleRate());
		designFilters();
	}
	void stopNote(float /*velocity*/, bool allowEnvelope) override
	{
		if (allowEnvelope)
		{
				currEnvState = Release;
		}
		else
		{
			clearCurrentNote();
			tableDelta = 0.0f; 
		}
	}
	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}
	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
	{
		if (tableDelta != 0.0) // ***
		{
			tempBuffer.setSize(outputBuffer.getNumChannels(), numSamples);
			tempBuffer.clear();
				for (int indexSample = 0; indexSample < numSamples; indexSample++)
				{
					float currentSample = getNextSample() * level * currEnvelope;
					//auto currentSample = (2.0f * (float)rand() / (float)RAND_MAX - 1.0f) * level * tailOff;
					for (int i = tempBuffer.getNumChannels(); --i >= 0;)
						tempBuffer.addSample(i, indexSample, currentSample);					

					switch (currEnvState)
					{
					case Attack:
					{
						currEnvelope += envelopeChange[currEnvState];
						if (currEnvelope > .9995f)
							currEnvState = Decay;
						break;
					}
					case Decay:
					{
						currEnvelope *= envelopeChange[currEnvState];
						if (currEnvelope < sustainLvl)
							currEnvState = Sustain;
						break;
					}
					case Release:
					{
						currEnvelope *= envelopeChange[currEnvState];
						if (currEnvelope < noteOffLvl)
						{
							clearCurrentNote();
							tableDelta = 0.0f;
							currEnvState = Attack;
						}
						break;
					}
					}
				}
				
			for (int f = 0; f < numOfFilters; f++)
			{
				filterL[f].processSamples(tempBuffer.getWritePointer(0), numSamples);
				filterR[f].processSamples(tempBuffer.getWritePointer(1), numSamples);
			}
			outputBuffer.addFrom(0, startSample, tempBuffer.getReadPointer(0), numSamples);
			outputBuffer.addFrom(1, startSample, tempBuffer.getReadPointer(1), numSamples);
		}
	}
private:
	enum envelopeState {Attack, Decay, Sustain, Release};
	envelopeState currEnvState = Attack;
	float currEnvelope = 0.0f;
	float sustainLvl = 0.75f, noteOffLvl = 0.0005f;
	float envelopeChange[4] = { 0.0003f, 0.992f, 1.0f, 0.9995f };
	const AudioSampleBuffer& wavetable;
	float currentIndex = 0.0f, tableDelta = 0.0f,
		level = 0.0f, globalLevel = 0.0f;
	double currFrequency = 0.0;

	static const int numOfFilters = 3;
	double filterFreqRatio[numOfFilters] = { 1.0, 1.2, 3.0 };
	double filterQ[numOfFilters] = { 1.0, 1.0, 1.0 };
	float filterGain[numOfFilters] = { 1.0f, 1.0f, 1.0f };
	double filterFreq[numOfFilters] = { 0.0f, 0.0f, 0.0f };

	IIRFilter filterL [numOfFilters];
	IIRFilter filterR [numOfFilters];
	
	size_t coefNum = 0;
	AudioSampleBuffer tempBuffer;
};
