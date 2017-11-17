/*
  ==============================================================================

    MainComponent.cpp
    Authors: Jonas Holfelt
             Gergely Csapo
             Michael Castanieto

    Description:  Main GUI and audio component that contains all child components,
                  sets up the I/O for the audio device, and handles the real-time 
                  audio playback functionality.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorder.h"
#include "PlayComponent.h"
#include "RecComponent.h"
//#include "Mapper.h"  // TODO: <-- this is only used for testing!
#include "AudioProcessorBundler.h"

// used for initialising the deviceManager
static ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager; 

class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent() : deviceManager (getSharedAudioDeviceManager())
    {
        addAndMakeVisible (playComp);
        playComp.setSize (100, 100);
        addAndMakeVisible (recComp);
        recComp.setSize (100, 100);

        setSize(400, 400);
        setAudioChannels (1, 2);
                
        //initialize DSP blocks and assign parameters
        AudioProcessorBundler::initDSPBlocks();
        
        recorder = new AudioRecorder(3.f);
        // set recording functionality in the recording GUI component
        recComp.setRecorder(recorder);
        // setup the recorder to receive input from the microphone
        deviceManager.addAudioCallback(recorder);
    }

    ~MainContentComponent()
    {
        deviceManager.removeAudioCallback (recorder);
        delete recorder;
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        readIndex = 0;

        isTriggered = &playComp.isPlaying;
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        if(playComp.initRead) // reset readIndex on new trigger
        {
            readIndex = 0;
            playComp.initRead = false;
        }

        bufferToFill.clearActiveBufferRegion(); // clearing the buffer frame BEFORE writing to it

        // play back the recorded audio segment
        if (readIndex < recorder->getSampBuff().getNumSamples() && playComp.isPlaying)
        {
            const int numInputChannels = recorder->getNumChannels();
            const int numOutputChannels = bufferToFill.buffer->getNumChannels();

            int outputSamples = bufferToFill.buffer->getNumSamples(); // number of samples need to be output next frame
            writeIndex = bufferToFill.startSample; // write index, which is passed to the copyFrom() function

            // run this until the frame buffer is filled and the readindex does not exceeded the input
            while(outputSamples > 0 && readIndex != recorder->getSampBuff().getNumSamples())
            {
                int samplesToProcess = jmin(outputSamples, recorder->getSampBuff().getNumSamples() - readIndex);
            
                for (int ch = 0; ch < numOutputChannels; ch++) // iterate through output channels
                {
                    bufferToFill.buffer->copyFrom(
                        ch, // destination channel
                        writeIndex, // destination sample
                        recorder->getSampBuff(), // source buffer
                        ch % numInputChannels, // source channel
                        readIndex, // source sample
                        samplesToProcess); // number of samples to copy
                }

                outputSamples -= samplesToProcess; // decrement the number of output samples required to be written into the framebuffer
                readIndex += samplesToProcess;
                writeIndex += samplesToProcess;
            }
        }
        else if (playComp.isPlaying)
        {
            // Component call isn't being called in a message thread,
            // make sure it is thread-safe by temporarily suspending the message thread
            const MessageManagerLock mmLock;
            playComp.isPlaying = false;
            readIndex = 0;
        }
        else if (!playComp.isPlaying)
        {
            readIndex = 0;
        }

        AudioProcessorBundler::gain->process(*bufferToFill.buffer);
        //AudioProcessorBundler::timeStretch->process(*bufferToFill.buffer);
        AudioProcessorBundler::lopass->process(*bufferToFill.buffer);
        AudioProcessorBundler::hipass->process(*bufferToFill.buffer);
        
        
        float **outputFrame = bufferToFill.buffer->getArrayOfWritePointers();

        for (int samp = 0; samp < bufferToFill.buffer->getNumSamples(); ++samp)
        {
            for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch)
            {
                if(playComp.togSpaceComp.getToggleSpace() == 2) // impulse
                outputFrame[ch][samp] *= playComp.env.envelope(10, 0.8, 2000, *isTriggered); // APR
                else // sustain
                outputFrame[ch][samp] *= playComp.env.envelope(10, 0.8, 500, 0.5, 2000, *isTriggered); // APDSR
            }
        }
        
    }

    void releaseResources() override
    {

    }   

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        playComp.setBounds (0, 0, getWidth(), getHeight() * 3 / 4);
        recComp.setBounds (0, getHeight() * 3 / 4, getWidth(), getHeight() * 1 / 4);
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

private:
    /* This function sets up the I/O to stream audio to/from a device
     * RuntimePermissions::recordAudio requests the microphone be used as audio input
     */
    AudioDeviceManager& getSharedAudioDeviceManager()  
    {  
        if (sharedAudioDeviceManager == nullptr)
        {
            sharedAudioDeviceManager = new AudioDeviceManager();
            RuntimePermissions::request (RuntimePermissions::recordAudio, runtimePermissionsCallback);
        }
        return *sharedAudioDeviceManager;
    }
    
    // checks to see if the request for microphone access was granted
    static void runtimePermissionsCallback (bool wasGranted)
    {
        int numInputChannels = wasGranted ? 2 : 0;
        sharedAudioDeviceManager->initialise (numInputChannels, 2, nullptr, true, String(), nullptr);
    }
     
    //==============================================================================
    PlayComponent playComp;
    RecComponent recComp;
    AudioRecorder *recorder; // recording from the devices microphone to an AudioBuffer
    AudioDeviceManager& deviceManager; // manages audio I/O devices 
    int readIndex;
    int writeIndex;
    int sampleRate;
    bool* isTriggered;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent(){ return new MainContentComponent();
}
