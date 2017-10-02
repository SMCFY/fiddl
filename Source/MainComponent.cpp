#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorder.h"

static ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager; /* used for initialising
                                                                      the deviceManager */
class MainContentComponent   : public AudioAppComponent,
                               public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent()
       : deviceManager (getSharedAudioDeviceManager())
    {
        addAndMakeVisible(recordButton);
        recordButton.setButtonText("Record");
        recordButton.addListener (this);

        addAndMakeVisible(playButton);
        playButton.setButtonText("Play");
        playButton.addListener(this);

        setSize (400,400);
        
        recorder = new AudioRecorder();

        //testing
        isPlaying = false;
        totalSamples = 0;

        sampBuff.setSize(0,0);
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
          
        //deviceManager needs to be setup for the recorder
        deviceManager.addAudioCallback (recorder);
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
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.
        
        // initialising sample rate in the recorder
        recorder->setSampleRate(sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {

        // I/O channels acquired from sample buffer and the framebuffer
        const int numInputChannels = recorder->sampBuff->getNumChannels();
        const int numOutputChannels = bufferToFill.buffer->getNumChannels();

        int outputSamples = bufferToFill.buffer->getNumSamples(); // init the number of samples need to be output

        int writeIndex = bufferToFill.startSample; // init write index for target buffer

        while(outputSamples > 0){ // run this until the frame buffer is filled

            int samplesToProcess = jmin(outputSamples, recorder->sampBuff->getNumSamples()-readIndex); // returns the appropriate number of samples that need to be processed from the sample buffer


            for (int ch = 0; ch < numOutputChannels; ch++) // iterate through output channels
                {
                    // reading from the sample buffer to the framebuffer
                    // The sample buffer is coming from the AudioRecorder buffer
                    bufferToFill.buffer->copyFrom(
                        ch, // destination channel
                        writeIndex, // destination sample
                        *recorder->sampBuff, // source buffer
                        ch % numInputChannels, // source channel
                        readIndex, // source sample
                        samplesToProcess); // number of samples to copy
                }
                
            outputSamples -= samplesToProcess; // decrement the number of output samples rquired to be written into the framebuffer

            // increment read and write index
            readIndex += samplesToProcess;
            writeIndex += samplesToProcess;
        }

        readIndex = 0;


        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.
        
        sampBuff.setSize (0, 0); // deallocate memory by resizing the buffer to 0x0
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
        recordButton.setBounds (0, 0, getWidth()/2, getHeight());
        playButton.setBounds (getWidth()/2, 0, getWidth()/2, getHeight());
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

//    void buttonClicked (Button* button) override // point to the button class and call the button voids
//    {
//        if (button == &recordButton) recordButtonClicked();
//        if (button == &playButton) playButtonClicked();
//    }
    
    /* A buttonClicked() method needs to be implemented.
     * Something along the lines of the following method would be nice:
     * 
    void buttonClicked (Button* button) override
    {
        if (button == &recordButton)
        {
            if (recorder.isRecording())
                stopRecording();
            else
                startRecording();
        }
    }
    */

private:
    void startRecording ()
    {
        recorder->startRecording ();

        recordButton.setButtonText ("Stop");
    }

    void stopRecording()
    {
        totalSamples = recorder->writeIndex;
        recorder->stop();
        recordButton.setButtonText ("Record");
    }
    
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

    void startPlaying ()
    {
        playButton.setButtonText ("Stop");
        isPlaying = true;
    }

    void stopPlaying()
    {
        
        playButton.setButtonText ("Play");       
        isPlaying = false;
    }

    void buttonClicked (Button* button) override //void for what happens when clicking a button
    {
        if (button == &recordButton)
        {
            if (recorder->isRecording())
                stopRecording();
            else
                startRecording();
        }
        
        if(button == &playButton)
        {
            if(isPlaying)
                stopPlaying();
            else
                startPlaying();
        }
    }
    
    


  //  void playButtonClicked() //void for what happens when clicking play
   // {
        //if playing, stop else play
   // }
    //==============================================================================
    TextButton recordButton; //declaring buttons
    TextButton playButton;
    Boolean isPlaying;

    AudioSampleBuffer sampBuff; // sample buffer, where the recordings are stored
    int readIndex; // read index of the sample buffer
    AudioDeviceManager& deviceManager; // manages audio I/O devices 
    AudioRecorder *recorder; // recording from a device to a file
    int totalSamples;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
