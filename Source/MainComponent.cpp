#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorder.h"
#include "PlayComponent.h"
#include "RecComponent.h"

// used for initialising the deviceManager
static ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager; 

class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent() : deviceManager(getSharedAudioDeviceManager())
    {
        
        addAndMakeVisible(playComp);
        playComp.setSize(100, 100);
        addAndMakeVisible(recComp);
        recComp.setSize(100, 100);

        setSize(400, 400);
        setAudioChannels(2, 2);
        
        readIndex = 0;
        
        // sampleRate is hard coded for now
        // this is because the recorder cannot be initialised in prepareToPlay()
        // where the real sampleRate can be used (assumed to be 44.1K)
        int sampleRate = 44100;
        
        recorder = new AudioRecorder(sampleRate, 2, 3.f);
        recorder->setSampleRate(sampleRate);
        recComp.setRecorder(recorder);
        deviceManager.addAudioCallback(recorder);
    }

    ~MainContentComponent ()
    {
        deviceManager.removeAudioCallback(recorder);
        delete recorder;
        
        //delete playComp;
        //delete recComp;
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {

    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion (); // clearing the buffer frame BEFORE writing to it
        if (readIndex < recorder->getBufferLengthInSamples () && playComp.isPlaying)
        {
            const int numInputChannels = recorder->getNumChannels ();
            const int numOutputChannels = bufferToFill.buffer->getNumChannels ();

            int outputSamples = bufferToFill.buffer->getNumSamples (); // number of samples need to be output next frame
            writeIndex = bufferToFill.startSample; // write index, which is passed to the copyFrom() function

            while(outputSamples > 0 && readIndex != recorder->getSampBuff().getNumSamples()) // run this until the frame buffer is filled and the readindex does not exceeded the input
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
                std::cout << recorder->getRecBuff()[0][readIndex] << std::endl;

                outputSamples -= samplesToProcess; // decrement the number of output samples rquired to be written into the framebuffer
                readIndex += samplesToProcess;
                writeIndex += samplesToProcess;
            }
        }
        else if (playComp.isPlaying)
        {
            // Component call isn't being called in a message thread,
            // make sure it is thread-safe by temporarily suspending the message thread
            const MessageManagerLock mmLock;
            playComp.stopPlaying();
            readIndex = 0;
        }
        else if (!playComp.isPlaying)
             readIndex = 0;

        // bufferToFill.buffer->applyGain(playComp.y); // mapping of finger position ot gain
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.
    }   

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        playComp.setBounds (0, 0, getWidth(), getHeight()*3/4);
        recComp.setBounds (0, getHeight()*3/4, getWidth(), getHeight()*1/4);
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }
/*
    void mouseDown (const MouseEvent &event) override
    {
         
        if (recComp.isRecording)
        {
           
            recorder->startRecording();
        }
    }

    void mouseUp (const MouseEvent &event) override
    {
        std::cout << "record" << std::endl;
        if (!recComp.isRecording)
            {
            recorder->stop();
        }
        if (!playComp.isPlaying)
        {
            readIndex = 0;
        }
    }*/

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
    AudioRecorder *recorder; // recording from a device to a file
    AudioDeviceManager& deviceManager; // manages audio I/O devices 

    int readIndex;
    int writeIndex;
    int sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent(){ return new MainContentComponent(); }
