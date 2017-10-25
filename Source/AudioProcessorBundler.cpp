/*
  ==============================================================================

    AudioProcessorBundler.cpp
    Created: 24 Oct 2017 7:02:53pm
    Author:  geri

  ==============================================================================
*/

#include "AudioProcessorBundler.h"
#include "Mapper.h"

Gain *AudioProcessorBundler::gain = new Gain(Mapper::gainLevel);
//Gain *gain.addParameter(Mapper::gainLevel);

AudioBuffer<float> AudioProcessorBundler(AudioBuffer<float> buff)
{
	// apply AudioProcessors here 
    return buff;
}
