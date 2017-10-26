/*
  ==============================================================================

    AudioProcessorBundler.h
    Created: 24 Oct 2017 7:02:53pm
    Author:  geri

    Description:  The AudioProcessor objects are declared and used here. 

  ==============================================================================
*/

#pragma once

#include "Gain.h"

class AudioProcessorBundler
{
	public:
<<<<<<< HEAD
		static AudioBuffer<float>* processBuffer(AudioBuffer<float> *buff);
=======
		static AudioBuffer<float> processBuffer(AudioBuffer<float> buff);
>>>>>>> 867e26ad08a567b9b337ad2e9b1984d43eac2b46

	//private:  <-- DSP processors are public so that Mapper has access to them
    //              in order to connect a processor to an AudioParameterFloat
		static Gain *gain;
};