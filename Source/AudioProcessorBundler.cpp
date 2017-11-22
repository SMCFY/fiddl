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
	// chain AudioProcessors here 
    return buff;
}

void AudioProcessorBundler::initDSPBlocks()
{
    // dsp parameters
    gainLevel = new AudioParameterFloat("gainLevel", "Gain", 0.0f, 1.0f, 0.5f);
    pitch = new AudioParameterFloat("pitch", "Pitch", 0.0f, 1.0f, 0.5f);
    tempo = new AudioParameterFloat("tempo", "Tempo", 0.0f, 1.0f, 0.5f);
    lowPassFilterFreqParam  = new AudioParameterFloat ("LPFREQ", "Post Lowpass Freq.", { 20.f, 3000.f, 0.f, 0.5f }, 3000.f, "Hz");
    highPassFilterFreqParam  = new AudioParameterFloat ("HPFREQ", "Pre Highpass Freq.", { 20.f, 3000.f, 0.f, 0.5f }, 3000.f, "Hz");
    bandPassFilterFreqParam  = new AudioParameterFloat ("BPFREQ", "Pre Bandpass Freq.", { 20.f, 3000.f, 0.f, 0.5f }, 3000.f, "Hz");
    
    // dsp blocks
    gain = new Gain(gainLevel);
    timeStretch = new TimeStretch(pitch, tempo);
    lopass = new Filter(lowPassFilterFreqParam, "lowpass");
    hipass = new Filter(highPassFilterFreqParam, "highpass");
    bapass = new Filter(bandPassFilterFreqParam, "bandpass");

    //ar = Envelope(44100, Envelope::ar);
    //adsr = Envelope(44100, Envelope::adsr);

    // add parameter        - all AudioParameterFloat objects must be connected to a DSP processor
    gain->addParameter(gainLevel);
    timeStretch->addParameter(pitch);
    timeStretch->addParameter(tempo);
    lopass->addParameter(lowPassFilterFreqParam);
    hipass->addParameter(highPassFilterFreqParam);
    bapass->addParameter(bandPassFilterFreqParam);
}

// DSP parameters:
AudioParameterFloat *AudioProcessorBundler::gainLevel;
AudioParameterFloat *AudioProcessorBundler::pitch;
AudioParameterFloat *AudioProcessorBundler::tempo;
AudioParameterFloat *AudioProcessorBundler::lowPassFilterFreqParam;
AudioParameterFloat *AudioProcessorBundler::highPassFilterFreqParam;
AudioParameterFloat *AudioProcessorBundler::bandPassFilterFreqParam;

// DSP processors:
Gain *AudioProcessorBundler::gain;
TimeStretch *AudioProcessorBundler::timeStretch;
Filter *AudioProcessorBundler::lopass;
Filter *AudioProcessorBundler::hipass;
Filter *AudioProcessorBundler::bapass;
