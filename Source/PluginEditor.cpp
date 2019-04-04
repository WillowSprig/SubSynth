/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SyntSubAudioProcessorEditor::SyntSubAudioProcessorEditor (SyntSubAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	keyboardComponent(processor.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(1000, 500);
	startTimer(400);
	//colours first, otherwise it won't work
	//quality
	qSlider[0].setColour(Slider::rotarySliderFillColourId, Colours::maroon);
	qSlider[1].setColour(Slider::rotarySliderFillColourId, Colours::steelblue);
	qSlider[2].setColour(Slider::rotarySliderFillColourId, Colours::goldenrod);
	qSlider[0].setColour(Slider::thumbColourId, Colours::maroon);
	qSlider[1].setColour(Slider::thumbColourId, Colours::steelblue);
	qSlider[2].setColour(Slider::thumbColourId, Colours::goldenrod);
	//envelope times
	qSlider[8].setColour(Slider::rotarySliderFillColourId, Colours::sienna);
	qSlider[9].setColour(Slider::rotarySliderFillColourId, Colours::powderblue);
	qSlider[10].setColour(Slider::rotarySliderFillColourId, Colours::khaki);
	qSlider[8].setColour(Slider::thumbColourId, Colours::sienna);
	qSlider[9].setColour(Slider::thumbColourId, Colours::powderblue);
	qSlider[10].setColour(Slider::thumbColourId, Colours::khaki);
	//frequency
	qSlider[3].setColour(Slider::trackColourId, Colours::maroon);
	qSlider[4].setColour(Slider::trackColourId, Colours::steelblue);
	qSlider[5].setColour(Slider::trackColourId, Colours::goldenrod);
	qSlider[3].setColour(Slider::thumbColourId, Colours::maroon);
	qSlider[4].setColour(Slider::thumbColourId, Colours::steelblue);
	qSlider[5].setColour(Slider::thumbColourId, Colours::goldenrod);
	//gain
	qSlider[6].setColour(Slider::trackColourId, Colours::steelblue);
	qSlider[7].setColour(Slider::trackColourId, Colours::goldenrod);
	qSlider[6].setColour(Slider::thumbColourId, Colours::steelblue);
	qSlider[7].setColour(Slider::thumbColourId, Colours::goldenrod);

	qSlider[3].setColour(Slider::backgroundColourId, Colours::dimgrey);

	for (int s = 0; s < numOfParameters; s++)
	{
		if (s < 3)
		{
			qSlider[s].setColour(Slider::rotarySliderOutlineColourId, Colours::dimgrey);
			qSlider[s].setRange(0.1, 10.0, 0.1);
			qSlider[s].setSliderStyle(Slider::Rotary);
		}
		else if (s==4 || s==5)
		{
			qSlider[s].setColour(Slider::backgroundColourId, Colours::dimgrey);
			qSlider[s].setRange(1.0, 10.0, 0.1);
			qSlider[s].setSliderStyle(Slider::LinearHorizontal);
		}
		else if (s>5 && s<8)
		{
			qSlider[s].setColour(Slider::backgroundColourId, Colours::dimgrey);
			qSlider[s].setRange(-30.0, 20.0, 0.5);
			qSlider[s].setSliderStyle(Slider::LinearVertical);
		}
		if (s >= 8)
		{
			qSlider[s].setColour(Slider::rotarySliderOutlineColourId, Colours::dimgrey);
			qSlider[s].setSliderStyle(Slider::Rotary);
		}
		qSlider[s].setTextBoxStyle(Slider::TextBoxBelow, false, qSlider[s].getTextBoxWidth() / 2, 20);
		qSlider[s].setPopupDisplayEnabled(false, false, this);
		qSlider[s].setValue(1.0);
		addAndMakeVisible(qSlider[s]);
		qSlider[s].addListener(this);
		//creating labels for each slider
		addAndMakeVisible(qLabel[s]);
		qLabel[s].setJustificationType(Justification::centred);
		qLabel[s].attachToComponent(&qSlider[s], false);
	}
	qSlider[3].setTextBoxStyle(Slider::TextBoxRight, false, qSlider[3].getTextBoxWidth(), 20);
	qSlider[4].setTextBoxStyle(Slider::TextBoxRight, false, qSlider[4].getTextBoxWidth(), 20);
	qSlider[5].setTextBoxStyle(Slider::TextBoxRight, false, qSlider[5].getTextBoxWidth(), 20);
	//setting range for lowpass filter
	qSlider[3].setRange(3.0, 30.0, 0.5);
	qSlider[3].setValue(5.0);
	qSlider[3].setSliderStyle(Slider::LinearHorizontal);
	//setting range for envelope times
	qSlider[8].setRange(0.1, 700.0, 0.5);
	qSlider[9].setRange(0.1, 500.0, 0.5);
	qSlider[10].setRange(0.1, 1000.0, 0.5);
	//labeling the sliders
	qSlider[0].setName("Quality0");
	qSlider[1].setName("Quality1");
	qSlider[2].setName("Quality2");
	qSlider[3].setName("Frequency0");
	qSlider[4].setName("Frequency1");
	qSlider[5].setName("Frequency2");
	qSlider[6].setName("Gain1");
	qSlider[7].setName("Gain2");
	qSlider[8].setName("Attack");
	qSlider[9].setName("Decay");
	qSlider[10].setName("Release");
	//labeling the labels
	qLabel[0].setText("f0 quality", dontSendNotification);
	qLabel[1].setText("f1 quality", dontSendNotification);
	qLabel[2].setText("f2 quality", dontSendNotification);
	qLabel[3].setText("f0 frequency ratio", dontSendNotification);
	qLabel[4].setText("f1 frequency ratio", dontSendNotification);
	qLabel[5].setText("f2 frequency ratio", dontSendNotification);
	qLabel[6].setText("f1 gain [dB]", dontSendNotification);
	qLabel[7].setText("f2 gain [dB]", dontSendNotification);
	qLabel[8].setText("Attack time [ms]", dontSendNotification);
	qLabel[9].setText("Decay time [ms]", dontSendNotification);
	qLabel[10].setText("Release time [ms]", dontSendNotification);
	//adding keyboard
	addAndMakeVisible(keyboardComponent);
}

SyntSubAudioProcessorEditor::~SyntSubAudioProcessorEditor()
{
}

//==============================================================================
void SyntSubAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("f0 - dolnoprzepustowy, f1,f2 - pasmowe", getLocalBounds(), Justification::centredRight, 1);
}

void SyntSubAudioProcessorEditor::resized()
{
	keyboardComponent.setBounds(10, 280, getWidth() - 20, getHeight() - 300);
	//rotary sliders for quality
	qSlider[0].setBounds(30, 50, 110, 80);
	qSlider[1].setBounds(150, 50, 110, 80);
	qSlider[2].setBounds(270, 50, 110, 80);
	//horizontal sliders for frequency ratio
	qSlider[3].setBounds(600, 50, 200, 60);
	qSlider[4].setBounds(600, 130, 200, 60);
	qSlider[5].setBounds(600, 210, 200, 60);
	//vertical sliders for gain
	qSlider[6].setBounds(400, 50, 70, 200);
	qSlider[7].setBounds(500, 50, 70, 200);
	//rotary sliders for envelope times
	qSlider[8].setBounds(30, 170, 110, 80);
	qSlider[9].setBounds(150, 170, 110, 80);
	qSlider[10].setBounds(270, 170, 110, 80);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SyntSubAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	String name = slider->getName();
	if (name == "Quality0")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 1, 0);
	}
	else if (name == "Quality1")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 1, 1);
	}
	else if (name == "Quality2")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 1, 2);
	}
	else if (name == "Frequency0")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 0, 0);
	}
	else if (name == "Frequency1")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 0, 1);
	}
	else if (name == "Frequency2")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 0, 2);
	}
	else if (name == "Gain1")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 2, 1);
	}
	else if (name == "Gain2")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 2, 2);
	}
	else if (name == "Attack")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 0);
	}
	else if (name == "Decay")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 1);
	}
	else if (name == "Release")
	{
		double value = slider->getValue();
		processor.synthAudioSource.passParameters(value, 3);
	}
}