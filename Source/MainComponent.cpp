
#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
        sampBuff.setSize(2,120000000);

        // specify the number of input and output channels that we want to open
        //setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {

        // I/O channels acquired from sample buffer and the framebuffer
        const int numInputChannels = sampBuff.getNumChannels();
        const int numOutputChannels = bufferToFill.buffer->getNumChannels();

        int outputSamples = bufferToFill.buffer->getNumSamples(); // init the number of samples need to be output

        int writeIndex = bufferToFill.startSample; // init write index for target buffer

        while(outputSamples > 0){ // run this until the frame buffer is filled

            int samplesToProcess = jmin(outputSamples, sampBuff.getNumSamples()-readIndex); // returns the appropriate number of samples that need to be processed from the sample buffer


            for (int ch = 0; ch < numOutputChannels; ch++) // iterate through output channels
                {
                    // reading from the sample buffer to the framebuffer
                    bufferToFill.buffer->copyFrom(
                        ch, // destination channel
                        writeIndex, // destination sample
                        sampBuff, // source buffer
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
        //bufferToFill.clearActiveBufferRegion();
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
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================

    AudioSampleBuffer sampBuff; // sample buffer, where the recordings are stored
    int readIndex; // read index of the sample buffer
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
