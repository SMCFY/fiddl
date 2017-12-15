/*
  ==============================================================================

    AudioRecorder.h
    Created: 27 Sep 2017 1:30:17pm
    Authors: Michael Castanieto
             Jonas Holfelt
             Gergely Csapo

    Description: Sets up the functionality for real-time audio recording from
                 the device's microphone to an AudioBuffer. When recording has
                 stopped, audio is truncated to remove silence from the beginning
                 and end of the audio recording.
                 
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioRecorder : public AudioIODeviceCallback
{
    public:
        AudioRecorder (float bufferLengthInSeconds, AudioThumbnail& thumbnailToUpdate);
        ~AudioRecorder();
        
        void startRecording(); 
        void stop();
        bool isRecording() const;
        void audioDeviceAboutToStart (AudioIODevice* device) override;
        void audioDeviceStopped() override;
        // callback function for recording audio from the mic to the buffer
        void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                    float** outputChannelData, int numOutputChannels,
                                    int numSamples) override;
        int getSampleRate();
        int getNumChannels();
        float** getRecBuff();
        AudioBuffer<float> getSampBuff();
        int getBufferLengthInSamples();
        int getWriteIndex();
        int getSampLength();

        float centroid;
    private:
        /* audio is truncated according to a threshold which sets the
           buffer read index to start reading above the threshold and
           to stop reading when audio goes below the threshold at the 
           end of the recording */
        void truncate(float** recording, float threshold);
        float spectralCentroid(float* buff);

        AudioBuffer<float> sampBuff;
        int numChannels;
        float **recBuff; // sample buffer, where the recordings are stored
        float *specBuff; // copy of the truncated recording for FFT
        float bufferLengthInSeconds;
        int bufferLengthInSamples;
        double sampleRate;
        int writeIndex; // index where the recording buffer is being written to
        CriticalSection writerLock;
        Boolean activeWriter; // true when the buffer is being written to
        int sampStart; // start index of truncated sample
        int sampLength; // length of truncated sample
    
        AudioThumbnail& thumbnail;
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorder);
};
