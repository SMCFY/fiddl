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

void AudioProcessorBundler::initDSPBlocks(int samplingRate)
{
    // dsp parameters
    gainLevel = new AudioParameterFloat("gainLevel", "Gain", 0.0f, 1.0f, 1.0f);
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

    ar = Envelope(samplingRate, Envelope::AR);
    adsr = Envelope(samplingRate, Envelope::ADSR);

    // add parameter        - all AudioParameterFloat objects must be connected to a DSP processor
    gain->addParameter(gainLevel);
    timeStretch->addParameter(pitch);
    timeStretch->addParameter(tempo);
    lopass->addParameter(lowPassFilterFreqParam);
    hipass->addParameter(highPassFilterFreqParam);
    bapass->addParameter(bandPassFilterFreqParam);
    
    // set process switches
    gainIsEnabled = false;
    pitchIsEnabled = false;
    tempoIsEnabled = false;
    lowPassISEnabled = false;
    highPassIsEnabled = false;
    bandPassIsEnabled = false;
}

void AudioProcessorBundler::turnOffProcessors()
{
    gainIsEnabled = false;
    pitchIsEnabled = false;
    tempoIsEnabled = false;
    lowPassISEnabled = false;
    highPassIsEnabled = false;
    bandPassIsEnabled = false;
}

void AudioProcessorBundler::turnOnProcessor(ProcessorSwitch processorSwitch)
{
    switch (processorSwitch)
    {
        case GAIN_ON:
            gainIsEnabled = true;
            break;
        case PITCH_ON:
            pitchIsEnabled = true;
            break;
        case TEMPO_ON:
            tempoIsEnabled = true;
            break;
        case LOWPASS_ON:
            lowPassISEnabled = true;
            break;
        case HIGHPASS_ON:
            highPassIsEnabled = true;
            break;
        case BANDPASS_ON:
            bandPassIsEnabled = true;
            break;
    }
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

Envelope AudioProcessorBundler::ar;
Envelope AudioProcessorBundler::adsr;

// DSP processor switches:
bool AudioProcessorBundler::gainIsEnabled;
bool AudioProcessorBundler::pitchIsEnabled;
bool AudioProcessorBundler::tempoIsEnabled;
bool AudioProcessorBundler::lowPassISEnabled;
bool AudioProcessorBundler::highPassIsEnabled;
bool AudioProcessorBundler::bandPassIsEnabled;
