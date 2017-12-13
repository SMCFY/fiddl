/*
  ==============================================================================

    DSP.cpp
    Created: 20 Oct 2017 12:54:24pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#include "DSP.h"

DSP::DSP()
{
}

DSP::~DSP()
{
}

const String DSP::getName() const
{
	return "fiddl";
}

void DSP::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock)
{
}

void DSP::releaseResources()
{
}

void DSP::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	process(buffer);
}

const String DSP::getInputChannelName(int channelIndex) const
{
    return "";
}

const String DSP::getOutputChannelName(int channelIndex) const
{
    return "";
}

bool DSP::isInputChannelStereoPair(int index) const
{
    return true;
}

bool DSP::isOutputChannelStereoPair(int index) const
{
    return true;
}

bool DSP::silenceInProducesSilenceOut() const
{
    return true;
}

double DSP::getTailLengthSeconds() const
{
	return 0.0;
}

bool DSP::acceptsMidi() const
{
	return false;
}

bool DSP::producesMidi() const
{
	return false;
}

AudioProcessorEditor* DSP::createEditor()
{
	return NULL;
}

bool DSP::hasEditor() const
{
    return false;
}

int DSP::getNumPrograms()
{
    return 1;
}

int DSP::getCurrentProgram()
{
    return 0;
}

void DSP::setCurrentProgram(int index)
{
}

const String DSP::getProgramName(int index)
{
    return String();
}

void DSP::changeProgramName(int index, const String& newName)
{
}

void DSP::getStateInformation (juce::MemoryBlock& destData)
{
}

void DSP::setStateInformation(const void *data, int sizeInBytes)
{
}

bool DSP::supportsDoublePrecisionProcessing() const
{
    return false;
}
