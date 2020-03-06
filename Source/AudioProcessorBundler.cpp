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

void AudioProcessorBundler::initDSPBlocks(int sampleRate)
{
    // dsp parameters
    gainLevel = new AudioParameterFloat("gainLevel", "Gain", 0.0f, 1.0f, 1.0f);
    pitch = new AudioParameterFloat("pitch", "Pitch", -12.0f, 24.0f, 1.0f);
    tempo = new AudioParameterFloat("tempo", "Tempo", 0.0f, 1.0f, 0.5f);
    lowPassFilterFreqParam  = new AudioParameterFloat ("LPFREQ", "Post Lowpass Freq.", { 20.f, 3000.f, 0.f, 0.5f }, 3000.f, "Hz");
    lowPassFilterQParam = new AudioParameterFloat("LPQ", "Lowpass Q", { 0.1f, 100.0f, 0.f, 1.f }, 50.0f, "Q");
    highPassFilterFreqParam  = new AudioParameterFloat ("HPFREQ", "Pre Highpass Freq.", { 20.f, 3000.f, 0.f, 0.5f }, 3000.f, "Hz");
    highPassFilterQParam = new AudioParameterFloat("HPQ", "Highpass Q", { 0.1f, 100.0f, 0.f, 1.f }, 50.0f, "Q");
    bandPassFilterFreqParam  = new AudioParameterFloat ("BPFREQ", "Pre Bandpass Freq.", { 20.f, 3000.f, 0.f, 0.5f }, 3000.f, "Hz");
    bandPassFilterQParam = new AudioParameterFloat("BPQ", "Bandpass Q", { 0.1f, 100.0f, 0.f, 1.f }, 50.0f, "Q");
    roomSize = new AudioParameterFloat("roomSize", "Room Size", 0.0f, 1.0f, 0.5f);
    damping = new AudioParameterFloat("damping", "Damping", 0.0f, 1.0f, 0.4f);
    wetLevel = new AudioParameterFloat("wetLevel", "Wet Level", 0.0f, 1.0f, 0.7f);
    dryLevel = new AudioParameterFloat("dryLevel", "Dry Level", 0.0f, 1.0f, 0.3f);
    width = new AudioParameterFloat("width", "Width", 0.0f, 1.0f, 0.4f);
    freezeMode = new AudioParameterFloat("freezeMode", "Freeze Mode", 0.0f, 1.0f, 0.4f);
    
    // dsp blocks
    gain = new Gain(gainLevel);
    timeStretch = new TimeStretch(pitch, tempo, sampleRate); 
    lopass = new Filter(lowPassFilterFreqParam, lowPassFilterQParam, "lowpass", sampleRate);
    hipass = new Filter(highPassFilterFreqParam, highPassFilterQParam, "highpass", sampleRate);
    bapass = new Filter(bandPassFilterFreqParam, bandPassFilterQParam, "bandpass", sampleRate);
    reverb = new Reverberation(roomSize, damping, wetLevel, dryLevel, width, freezeMode, sampleRate);

    // add parameter        - all AudioParameterFloat objects must be connected to a DSP processor
    gain->addParameter(gainLevel);
    timeStretch->addParameter(pitch);
    timeStretch->addParameter(tempo);
    lopass->addParameter(lowPassFilterFreqParam);
    lopass->addParameter(lowPassFilterQParam);
    hipass->addParameter(highPassFilterFreqParam);
    hipass->addParameter(highPassFilterQParam);
    bapass->addParameter(bandPassFilterFreqParam);
    bapass->addParameter(bandPassFilterQParam);
    reverb->addParameter(roomSize);
    reverb->addParameter(damping);
    reverb->addParameter(wetLevel);
    reverb->addParameter(dryLevel);
    reverb->addParameter(width);
    reverb->addParameter(freezeMode);
    
    // set process switches
    gainIsEnabled = false;
    pitchIsEnabled = false;
    tempoIsEnabled = false;
    lowPassISEnabled = false;
    highPassIsEnabled = false;
    bandPassIsEnabled = false;
    reverbEnabled = false;
}

void AudioProcessorBundler::turnOffProcessors()
{
    gainIsEnabled = false;
    pitchIsEnabled = false;
    tempoIsEnabled = false;
    lowPassISEnabled = false;
    highPassIsEnabled = false;
    bandPassIsEnabled = false;
    reverbEnabled = false;
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
        case REVERB_ON:
            reverbEnabled = true;
            break;
    }
}


// DSP parameters:
AudioParameterFloat *AudioProcessorBundler::gainLevel;
AudioParameterFloat *AudioProcessorBundler::pitch;
AudioParameterFloat *AudioProcessorBundler::tempo;
AudioParameterFloat *AudioProcessorBundler::lowPassFilterFreqParam;
AudioParameterFloat *AudioProcessorBundler::lowPassFilterQParam;
AudioParameterFloat *AudioProcessorBundler::highPassFilterFreqParam;
AudioParameterFloat *AudioProcessorBundler::highPassFilterQParam;
AudioParameterFloat *AudioProcessorBundler::bandPassFilterFreqParam;
AudioParameterFloat *AudioProcessorBundler::bandPassFilterQParam;
AudioParameterFloat *AudioProcessorBundler::roomSize;
AudioParameterFloat *AudioProcessorBundler::damping;
AudioParameterFloat *AudioProcessorBundler::wetLevel;
AudioParameterFloat *AudioProcessorBundler::dryLevel;
AudioParameterFloat *AudioProcessorBundler::width;
AudioParameterFloat *AudioProcessorBundler::freezeMode;

// DSP processors:
Gain *AudioProcessorBundler::gain;
TimeStretch *AudioProcessorBundler::timeStretch;
Filter *AudioProcessorBundler::lopass;
Filter *AudioProcessorBundler::hipass;
Filter *AudioProcessorBundler::bapass;
Reverberation *AudioProcessorBundler::reverb;

// DSP processor switches:
bool AudioProcessorBundler::gainIsEnabled;
bool AudioProcessorBundler::pitchIsEnabled;
bool AudioProcessorBundler::tempoIsEnabled;
bool AudioProcessorBundler::lowPassISEnabled;
bool AudioProcessorBundler::highPassIsEnabled;
bool AudioProcessorBundler::bandPassIsEnabled;
bool AudioProcessorBundler::reverbEnabled;
