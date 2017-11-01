/*
  ==============================================================================

    AudioProcessorBundler.h
    Created: 24 Oct 2017 7:02:53pm
    Author:  geri

    Description:  The AudioProcessor objects are declared and initialized with
    their audio parameters here. The individual processing blocks are chained together
    in processBuffer.

  ==============================================================================
*/

#pragma once

#include "Gain.h"

class AudioProcessorBundler
{
	public:

		static AudioBuffer<float> processBuffer(AudioBuffer<float> buff);
        static void initDSPBlocks();

	//private:  <-- DSP processors are public so that MainContentComponent has access to them.
	//              AudioParameterFloats are public so that Mapper has access to them.
	
	    // DSP processors
		static Gain *gain;
		
		// DSP parameters
		static AudioParameterFloat *gainLevel;
};
