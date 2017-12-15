/*
  ==============================================================================

    TimeStretch.cpp
    Created: 20 Oct 2017 3:13:32pm
    Authors: Michael Castanieto
             Gergely Csapo
             Jonas Holfelt

  ==============================================================================
*/

#include "TimeStretch.h"

TimeStretch::TimeStretch(AudioParameterFloat *pitch, AudioParameterFloat *tempo, int sampleRate)
{
    this->pitch = pitch;
    this->tempo = tempo;
    pitchUpdated = false;
    
    timeStretchIndex = 0;
    
    counter = 0;

    soundTouch.setSampleRate(sampleRate);
    soundTouch.setChannels(1);
    soundTouch.setTempoChange(0.0f);
    soundTouch.setPitchSemiTones(0.0f);
    soundTouch.setRateChange(0.0f);
    soundTouch.setSetting(SETTING_USE_QUICKSEEK, 0);
    soundTouch.setSetting(SETTING_USE_AA_FILTER, 1);
    
    // settings for speech
    soundTouch.setSetting(SETTING_SEQUENCE_MS, 40);
    soundTouch.setSetting(SETTING_SEEKWINDOW_MS, 15);
    soundTouch.setSetting(SETTING_OVERLAP_MS, 8);
}

TimeStretch::~TimeStretch()
{

}

void TimeStretch::process(AudioBuffer<float> buffer)
{
    if (pitchUpdated)
    {
        soundTouch.setPitchSemiTones(pitch->get());
        pitchUpdated = false;
    }
    
    float **bufferFrame = buffer.getArrayOfWritePointers();
    int BUFF_SIZE = 512;
    float outputSamples[BUFF_SIZE];
    soundTouch.putSamples(*bufferFrame, buffer.getNumSamples());
    nSamples = soundTouch.receiveSamples(outputSamples, buffer.getNumSamples());
    for (int ch = 0; ch < buffer.getNumChannels(); ch++)
    {
        for (int sample = 0; sample < nSamples; sample++)
        {
            bufferFrame[ch][sample] = outputSamples[sample];
        }
    }
}

void TimeStretch::process(AudioBuffer<float> inputBuffer, AudioBuffer<float> outputBuffer, int &readIndex)
{
    if (tempoUpdated)
    {
        soundTouch.setPitchSemiTones(pitch->get());
        soundTouch.setPitchSemiTones(pitch->get());
    }
}
