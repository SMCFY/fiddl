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
    // dsp parameters
    gainLevel = new AudioParameterFloat("gainLevel", "Gain", 0.0f, 0.5f, 1.0f);
    pitch = new AudioParameterFloat("pitch", "Pitch", 0.0f, 0.5f, 1.0f);
    tempo = new AudioParameterFloat("tempo", "Tempo", 0.0f, 0.5f, 1.0f);
    
    // dsp blocks
    gain = new Gain(gainLevel);
    timeStretch = new TimeStretch(pitch, tempo);

    // add parameter
    // all AudioParameterFloat objects must be connected to a DSP processor
    gain->addParameter(gainLevel);
    timeStretch->addParameter(pitch);
    timeStretch->addParameter(tempo);
}

// DSP parameters:
AudioParameterFloat *AudioProcessorBundler::gainLevel;
AudioParameterFloat *AudioProcessorBundler::pitch;
AudioParameterFloat *AudioProcessorBundler::tempo;

// DSP processors:
Gain *AudioProcessorBundler::gain;
TimeStretch *AudioProcessorBundler::timeStretch;
