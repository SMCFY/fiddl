/*
  ==============================================================================

    AudioRecorder.cpp
    Created: 27 Sep 2017 1:30:17pm
    Author:  Michael Castanieto
             Jonas Holfelt
             Gergely Csapo

  ==============================================================================
*/

#include "AudioRecorder.h"

AudioRecorder::AudioRecorder (double sampleRate, int numChannels, double bufferLengthInSeconds)
    : activeWriter (false), writeIndex (0)
{
    setSampleRate(sampleRate);
    this->numChannels = numChannels;

    bufferLengthInSamples = ceil(sampleRate * bufferLengthInSeconds); 
    
    // sampBuff stores the recorded audio
    sampBuff = new float*[numChannels];
    for (int ch = 0; ch < numChannels; ch++)
    {
        sampBuff[ch] = new float[bufferLengthInSamples];
    }
}

AudioRecorder::~AudioRecorder()
{
    stop();
    for (int i = 0; i < numChannels; i++)
    {
        delete sampBuff[i];
    }
    delete sampBuff;
}

void AudioRecorder::startRecording ()
{
    stop();
    writeIndex = 0;
    
    if (sampleRate > 0)
    {
        const ScopedLock sl (writerLock);
        activeWriter = true;
    }
}

void AudioRecorder::stop()
{
    // First, clear this pointer to stop the audio callback from using our writer object..
    {
        const ScopedLock sl (writerLock);
        activeWriter = false;
        // clear the part of the buffer that is unused
        for (int ch = 0; ch < numChannels; ch++)
        {
            for (int sample = writeIndex; sample < getBufferLengthInSamples (); sample++)
            {
                sampBuff[ch][sample] = 0.0f;
            }
        }
        writeIndex = 0;
    }
}

bool AudioRecorder::isRecording() const
{
    return activeWriter != false;
}

void AudioRecorder::audioDeviceAboutToStart (AudioIODevice* device)
{
    sampleRate = device->getCurrentSampleRate();
}

void AudioRecorder::audioDeviceStopped() 
{
    sampleRate = 0;
}

void AudioRecorder::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                           float** outputChannelData, int numOutputChannels,
                                           int numSamples) 
{
    /* This is the callback function to record audio from the microphone to a buffer
     * When the play button is triggered, the buffer fills up with a maximum of 10 seconds
     * of recorded audio. 
     * sampBuff is a float array that stores the audio in each channel
     */
    if (activeWriter != false && writeIndex < bufferLengthInSamples-numSamples)
    {   
        int destStartSample = writeIndex; 
        const ScopedLock sl (writerLock);
        
        // store the recorded audio into sampBuff
        for (int ch = 0; ch < numChannels; ch++)
        {
            for (int sample= 0; sample < numSamples; sample++)
            {
                sampBuff[ch][sample+destStartSample] = inputChannelData[ch][sample];
            }
        }

        writeIndex+=numSamples;
    }
}

void AudioRecorder::setSampleRate (double sampleRate) 
{
    this->sampleRate = sampleRate;
}

int AudioRecorder::getNumChannels ()
{
    return numChannels;
}

float** AudioRecorder::getSampBuff () 
{
    return sampBuff;
}

int AudioRecorder::getBufferLengthInSamples ()
{
    return bufferLengthInSamples;
}

int AudioRecorder::getWriteIndex ()
{
    return writeIndex;
}
