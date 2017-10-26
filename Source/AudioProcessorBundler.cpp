<<<<<<< HEAD
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
=======
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
Gain *AudioProcessorBundler::gain = new Gain(Mapper::gainLevel);

>>>>>>> 867e26ad08a567b9b337ad2e9b1984d43eac2b46
