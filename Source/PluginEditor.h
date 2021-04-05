/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NoiseGeneratorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NoiseGeneratorAudioProcessorEditor (NoiseGeneratorAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~NoiseGeneratorAudioProcessorEditor() override;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NoiseGeneratorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGeneratorAudioProcessorEditor)

        juce::AudioProcessorValueTreeState& valueTreeState;
        juce::Slider mGainSlider { juce::Slider::LinearHorizontal, juce::Slider::TextBoxBelow };
        std::unique_ptr<SliderAttachment> gainAttachment;

};
