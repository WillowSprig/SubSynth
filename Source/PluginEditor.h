/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SyntSubAudioProcessorEditor  : public AudioProcessorEditor, private Timer, public Slider::Listener
{
public:
    SyntSubAudioProcessorEditor (SyntSubAudioProcessor&);
    ~SyntSubAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SyntSubAudioProcessor& processor;
	void timerCallback() override
	{
		keyboardComponent.grabKeyboardFocus();
		stopTimer();
	}
	static const int numOfParameters = 11;
	MidiKeyboardComponent keyboardComponent;
	Slider qSlider[numOfParameters];// = { Slider(), Slider(), Slider(), Slider(), Slider(), Slider(), Slider(), Slider(), Slider(), Slider(), Slider() };
	Label qLabel[numOfParameters];// = { Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()), Label(String(),String()) };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SyntSubAudioProcessorEditor)
};
