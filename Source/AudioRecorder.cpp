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
    
    // recBuff stores the recorded audio
    recBuff = new float*[numChannels];
    for (int ch = 0; ch < numChannels; ch++)
    {
        recBuff[ch] = new float[bufferLengthInSamples];
    }

}

AudioRecorder::~AudioRecorder()
{
    stop();
    for (int i = 0; i < numChannels; i++)
    {
        delete recBuff[i];
    }
    delete recBuff;
}

void AudioRecorder::startRecording ()
{
    //stop();
    
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
                recBuff[ch][sample] = 0.0f;
            }
        }
        writeIndex = 0;

        truncate(recBuff, 0.1f);
        sampBuff.setDataToReferTo(recBuff, numChannels, sampStart, sampLenght); //set the AudioBuffer pointer to the truncated segment
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
     * recBuff is a float array that stores the audio in each channel
     */
    if (activeWriter != false && writeIndex < bufferLengthInSamples-numSamples)
    {
       
        int destStartSample = writeIndex; 
        const ScopedLock sl (writerLock);
        
        // store the recorded audio into recBuff
        for (int ch = 0; ch < numChannels; ch++)
        {
            for (int sample= 0; sample < numSamples; sample++)
            {
                recBuff[ch][sample+destStartSample] = inputChannelData[ch][sample];
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

float** AudioRecorder::getRecBuff () 
{
    return recBuff;
}

AudioBuffer<float> AudioRecorder::getSampBuff () 
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

void AudioRecorder::truncate(float** recording, float threshold)
{
    int j = this->bufferLengthInSamples; //reversed iterator
    this->sampStart = 0;
    this->sampLenght = 0;

    for (int i = 0; i < this->bufferLengthInSamples; i++)
    {

        if(abs(recording[0][i]) > threshold && this->sampStart == 0){
            this->sampStart = i;
            std::cout << "startIndex: " << this->sampStart << std::endl;
        }

        if(abs(recording[0][j]) > threshold && this->sampLenght == 0){
            this->sampLenght = this->bufferLengthInSamples - (sampStart+this->bufferLengthInSamples-j);
            std::cout << "new length: " << this->sampLenght << std::endl;
        }

        j--;
    }
}
