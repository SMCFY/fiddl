/*
  ==============================================================================

    AudioRecorder.cpp
    Created: 27 Sep 2017 1:30:17pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#include "AudioRecorder.h"

AudioRecorder::AudioRecorder ()
    : backgroundThread ("Audio Recorder Thread"),
      sampleRate (0), nextSampleNum (0), activeWriter (nullptr), writeIndex (0)
{
    //backgroundThread.startThread();
    
    // sampBuff is an AudioSampleBuffer that stores the recorded audio
    sampBuff = new AudioSampleBuffer(2,441000);
    
    // tempBuff is the same as sampBuff, but stores audio as an array of floats for each channel
    tempBuff = new float*[2];
    for (int i = 0; i < 2; i++)
    {
        tempBuff[i] = new float[441000];
    }
}

AudioRecorder::~AudioRecorder()
{
    stop();
    for (int i = 0; i < 2; i++)
    {
        delete tempBuff[i];
    }
    delete tempBuff;
    delete sampBuff;
}

void AudioRecorder::startRecording ()
{
    stop();
    AudioSampleBuffer sampBuff; // sample buffer, where the recordings are stored
    writeIndex = 0;
    
    if (sampleRate > 0)
    {
        const ScopedLock sl (writerLock);
        activeWriter = new Boolean;
    }
}

void AudioRecorder::stop()
{
    // First, clear this pointer to stop the audio callback from using our writer object..
    {
        const ScopedLock sl (writerLock);
        activeWriter = nullptr;
        // clear the part of the buffer that is unused
        for (int ch = 0; ch < 2; ch++)
        {
            for (int sample = writeIndex; sample < 441000; sample++)
            {
                tempBuff[ch][sample] = 0.0f;
            }
        }
        writeIndex = 0;
    }
    
    // Now we can delete the writer object. It's done in this order because the deletion could
    // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
    // the audio callback while this happens.
    threadedWriter = nullptr;
}

bool AudioRecorder::isRecording() const
{
    return activeWriter != nullptr;
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
     * tempBuff is a float array that stores the audio in each channel
     */
    if (activeWriter != nullptr && writeIndex < sampBuff->getNumSamples()-numSamples)
    {   
        int destStartSample = writeIndex; 
        const ScopedLock sl (writerLock);
        
        // store the recorded audio into tempBuff
        for (int ch = 0; ch < 2; ch++)
        {
            for (int sample= 0; sample < numSamples; sample++)
            {
                tempBuff[ch][sample+destStartSample] = inputChannelData[ch][sample];
            }
        }
    
        // copy the recorded audio into sampBuff
        // TODO: currently does not copy! Values end up being all zeros.
        for (int destChannel = 0; destChannel < sampBuff->getNumChannels(); destChannel++) 
        {
            sampBuff->copyFrom (destChannel, destStartSample, inputChannelData[destChannel], numSamples);
        }
        
        writeIndex+=numSamples;
        
    }
}

void AudioRecorder::setSampleRate (double sampleRate) {
    this->sampleRate = sampleRate;
}
