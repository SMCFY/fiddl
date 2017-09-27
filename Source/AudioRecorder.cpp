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
      sampleRate (0), nextSampleNum (0), activeWriter (nullptr)
{
    backgroundThread.startThread();
}

AudioRecorder::~AudioRecorder()
{
    stop();
}

void AudioRecorder::startRecording (const File& file)
{
    stop();
    
    if (sampleRate > 0)
    {
        /* Create an OutputStream to write to our destination file...
         * This should eventually be replaced with a buffer, instead
         * of reading from a file
         */
        file.deleteFile();
        ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());
        
        if (fileStream != nullptr)
        {
            // Now create a WAV writer object that writes to our output stream...
            WavAudioFormat wavFormat;
            AudioFormatWriter* writer = wavFormat.createWriterFor (fileStream, sampleRate, 1, 16, StringPairArray(), 0);
            
            if (writer != nullptr)
            {
                fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)
                
                // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                // write the data to disk on our background thread.
                threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);
                
                nextSampleNum = 0;
                
                // And now, swap over our active writer pointer so that the audio callback will start using it..
                const ScopedLock sl (writerLock);
                activeWriter = threadedWriter;
            }
        }
    }
}

void AudioRecorder::stop()
{
    // First, clear this pointer to stop the audio callback from using our writer object..
    {
        const ScopedLock sl (writerLock);
        activeWriter = nullptr;
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