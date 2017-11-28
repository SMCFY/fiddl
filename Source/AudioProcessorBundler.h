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
#include "TimeStretch.h"
#include "Filter.h"
#include "Envelope.h"

enum ProcessorSwitch {GAIN_ON, PITCH_ON, TEMPO_ON, LOWPASS_ON, HIGHPASS_ON, BANDPASS_ON};

class AudioProcessorBundler
{
	public:

		static AudioBuffer<float> processBuffer(AudioBuffer<float> buff);
        static void initDSPBlocks(int samplingRate);
        static void turnOffProcessors();
        static void turnOnProcessor(ProcessorSwitch processorSwtich);

	//private:  <-- DSP processors are public so that MainContentComponent has access to them.
	//              AudioParameterFloats are public so that Mapper has access to them.
	
	    // DSP processors
		static Gain *gain;
        static TimeStretch *timeStretch;
        static Filter *lopass;
        static Filter *hipass;
        static Filter *bapass;

        static Envelope ar;
		static Envelope adsr;

		// DSP parameters
		static AudioParameterFloat *gainLevel;
        static AudioParameterFloat *pitch;
        static AudioParameterFloat *tempo;
        static AudioParameterFloat *lowPassFilterFreqParam;
        static AudioParameterFloat *lowPassFilterQParam;
        static AudioParameterFloat *highPassFilterFreqParam;
        static AudioParameterFloat *highPassFilterQParam;
        static AudioParameterFloat *bandPassFilterFreqParam;
        static AudioParameterFloat *bandPassFilterQParam;
    
        // DSP processor switches
        static bool gainIsEnabled;
        static bool pitchIsEnabled;
        static bool tempoIsEnabled;
        static bool lowPassISEnabled;
        static bool highPassIsEnabled;
        static bool bandPassIsEnabled;

};
