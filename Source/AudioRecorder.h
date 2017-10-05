/*
  ==============================================================================

    AudioRecorder.h
    Created: 27 Sep 2017 1:30:17pm
    Author:  Michael Castanieto
             Jonas Holfelt
             Gergely Csapo
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioRecorder : public AudioIODeviceCallback
{
    public:
        // constructer
        AudioRecorder(double sampleRate, int numChannels, double bufferLengthInSeconds);
        // destructor
        ~AudioRecorder();
        
        /** public methods **/
        void startRecording (); 
        void stop ();
        bool isRecording () const;
        void audioDeviceAboutToStart (AudioIODevice* device) override;
        void audioDeviceStopped () override;
        void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                    float** outputChannelData, int numOutputChannels,
                                    int numSamples) override;
        // setter methods
        void setSampleRate (double sampleRate);
        // getter methods
        double getSampleRate ();
        int getNumChannels ();
        float** getRecBuff ();
        AudioBuffer<float> getSampBuff ();
        int getBufferLengthInSamples ();
        int getWriteIndex ();


    private:  
        /** private variables **/
        AudioBuffer<float> sampBuff;
        int numChannels;
        float **recBuff; // sample buffer, where the recordings are stored
        int bufferLengthInSamples; // sample buffer length in integer seconds
        double sampleRate;
        int writeIndex; // index where the recording buffer is being written to
        CriticalSection writerLock;
        Boolean activeWriter;
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorder);
};
