/*
  ==============================================================================

    AudioProcessorBundler.cpp
    Created: 24 Oct 2017 7:02:53pm
    Author:  geri

  ==============================================================================
*/

#include "AudioProcessorBundler.h"
#include "Mapper.h"

AudioBuffer<float> AudioProcessorBundler::processBuffer(AudioBuffer<float> buff)
{
	// apply AudioProcessors here 
    return buff;
}

void AudioProcessorBundler::initDSPBlocks()
{
	//dsp blocks
    gain = new Gain(Mapper::gainLevel);

	//add parameter
	gain->addParameter(Mapper::gainLevel); // all AudioParameterFloat objects must be connected to a DSP processor

}

Gain *AudioProcessorBundler::gain;
