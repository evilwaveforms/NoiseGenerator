/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::String NoiseGeneratorAudioProcessor::paramGain("gain");

//==============================================================================
NoiseGeneratorAudioProcessor::NoiseGeneratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	),
#endif
	mState(*this, nullptr, juce::Identifier("NoiseGenerator"),
		{
			std::make_unique<juce::AudioParameterFloat>(paramGain,
														 "Gain",
														 0.0f,
														 1.0f,
														 0.2f)
		})

{
	gain = mState.getRawParameterValue(paramGain);
}

NoiseGeneratorAudioProcessor::~NoiseGeneratorAudioProcessor()
{
}

//==============================================================================
const juce::String NoiseGeneratorAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool NoiseGeneratorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool NoiseGeneratorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool NoiseGeneratorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double NoiseGeneratorAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int NoiseGeneratorAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int NoiseGeneratorAudioProcessor::getCurrentProgram()
{
	return 0;
}

void NoiseGeneratorAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String NoiseGeneratorAudioProcessor::getProgramName(int index)
{
	return {};
}

void NoiseGeneratorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void NoiseGeneratorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	previousGain = *gain;
}

void NoiseGeneratorAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoiseGeneratorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
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

void NoiseGeneratorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	auto currentGain = *gain * 1.0f;
	auto gainScale = currentGain * 2.0f;


	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());


	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);

		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
			channelData[sample] = random.nextFloat() * gainScale - currentGain;
		}
	}
}

//==============================================================================
bool NoiseGeneratorAudioProcessor::hasEditor() const
{
	return true;
}

juce::AudioProcessorEditor* NoiseGeneratorAudioProcessor::createEditor()
{
	return new NoiseGeneratorAudioProcessorEditor(*this, mState);
}

//==============================================================================
void NoiseGeneratorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	auto state = mState.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void NoiseGeneratorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mState.state.getType()))
			mState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new NoiseGeneratorAudioProcessor();
}
