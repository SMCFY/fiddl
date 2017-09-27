/*
  ==============================================================================

    AudioRecorder.h
    Created: 27 Sep 2017 1:30:17pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioRecorder : public AudioIODeviceCallback
{
    public:
        // constructer
        AudioRecorder();
        // destructor
        ~AudioRecorder();
        
        // public methods
        void startRecording (const File& file); /* I eventually want to replace this parameter
                                                 * with a buffer instead of reading from a file
                                                 */
        void stop ();
        bool isRecording () const;
        void audioDeviceAboutToStart (AudioIODevice* device) override;
        void audioDeviceStopped () override;
        void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                    float** outputChannelData, int numOutputChannels,
                                    int numSamples) override;
        
    private:  
        // private variables
        TimeSliceThread backgroundThread; // the thread that will write audio data to disk
        ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
        double sampleRate;
        int64 nextSampleNum;
        
        CriticalSection writerLock;
        AudioFormatWriter::ThreadedWriter* volatile activeWriter;
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorder);
};