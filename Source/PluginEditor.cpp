/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
NoiseGeneratorAudioProcessorEditor::NoiseGeneratorAudioProcessorEditor (NoiseGeneratorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    setSize (400, 800);

    mGainSlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(mGainSlider);
    gainAttachment.reset(new SliderAttachment(valueTreeState, "gain", mGainSlider));
}

NoiseGeneratorAudioProcessorEditor::~NoiseGeneratorAudioProcessorEditor()
{
}

//==============================================================================
void NoiseGeneratorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Volume", getWidth() / 2 - 50, getHeight() / 2 + 220, 100, 30, juce::Justification::centred, 1);
}

void NoiseGeneratorAudioProcessorEditor::resized()
{
    mGainSlider.setBounds(getWidth() / 2 - 200, getHeight() / 2 + 250, 400, 50);
}
