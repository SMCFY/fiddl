/*
  ==============================================================================

    AudioProcessorBundler.cpp
    Created: 24 Oct 2017 7:02:53pm
    Author:  geri

  ==============================================================================
*/

#include "AudioProcessorBundler.h"
#include "Mapper.h"

AudioBuffer<float>* AudioProcessorBundler::processBuffer(AudioBuffer<float> *buff)
{
	// apply AudioProcessors here 
	gain->process(buff);
    return buff;
}

Gain* AudioProcessorBundler::gain = new Gain(Mapper::gainLevel);
