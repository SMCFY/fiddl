#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public AudioAppComponent,
                               public ChangeListener,
                               public Button::Listener,
                               public Timer
{
public:
    
    MainContentComponent()
    :   state (Stopped)
    {
        setLookAndFeel (&lookAndFeel);
        
        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open...");
        openButton.addListener (this);
        
        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        playButton.addListener (this);
        playButton.setColour (TextButton::buttonColourId, Colours::green);
        playButton.setEnabled (false);

        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.addListener (this);
        stopButton.setColour (TextButton::buttonColourId, Colours::red);
        stopButton.setEnabled (false);

        addAndMakeVisible (&loopingToggle);
        loopingToggle.setButtonText ("Loop");
        loopingToggle.addListener (this);
        
        addAndMakeVisible (&currentPositionLabel);
        currentPositionLabel.setText ("Stopped", dontSendNotification);
        
        setSize (300, 200);
        
        formatManager.registerBasicFormats();
        transportSource.addChangeListener (this);

        setAudioChannels (2, 2);
        startTimer (20);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }
        
        transportSource.getNextAudioBlock (bufferToFill);
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    void resized() override
    {
        openButton.setBounds (10, 10, getWidth() - 20, 20);
        playButton.setBounds (10, 40, getWidth() - 20, 20);
        stopButton.setBounds (10, 70, getWidth() - 20, 20);
        loopingToggle.setBounds(10, 100, getWidth() - 20, 20);
        currentPositionLabel.setBounds (10, 130, getWidth() - 20, 20);
    }
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
        {
            if (transportSource.isPlaying())
                changeState (Playing);
            else
                changeState (Stopped);
        }
    }

    void buttonClicked (Button* button) override
    {
        if (button == &openButton)      openButtonClicked();
        if (button == &playButton)      playButtonClicked();
        if (button == &stopButton)      stopButtonClicked();
        if (button == &loopingToggle)   loopButtonChanged();
    }
    
    void timerCallback() override
    {
        if (transportSource.isPlaying())
        {
            const RelativeTime position (transportSource.getCurrentPosition());
            
            const int minutes = ((int) position.inMinutes()) % 60;
            const int seconds = ((int) position.inSeconds()) % 60;
            const int millis  = ((int) position.inMilliseconds()) % 1000;
            
            const String positionString (String::formatted ("%02d:%02d:%03d", minutes, seconds, millis));
            
            currentPositionLabel.setText (positionString, dontSendNotification);
        }
        else
        {
            currentPositionLabel.setText ("Stopped", dontSendNotification);
        }
    }
    
    void updateLoopState (bool shouldLoop)
    {
        if (readerSource != nullptr)
            readerSource->setLooping (shouldLoop);
    }


private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    
    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;
            
            switch (state)
            {
                case Stopped:
                    stopButton.setEnabled (false);
                    playButton.setEnabled (true);
                    transportSource.setPosition (0.0);
                    break;
                    
                case Starting:
                    playButton.setEnabled (false);
                    transportSource.start();
                    break;
                
                case Playing:
                    stopButton.setEnabled (true);
                    break;
                
                case Stopping:
                    transportSource.stop();
                    break;
            }
        }
    }
    
    void openButtonClicked()
    {
        FileChooser chooser ("Select a Wave file to play...",
                             File::nonexistent,
                             "*.wav");
        
        if (chooser.browseForFileToOpen())
        {
            File file (chooser.getResult());
            AudioFormatReader* reader = formatManager.createReaderFor (file);
            
            if (reader != nullptr)
            {
                ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
                transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
                playButton.setEnabled (true);
                readerSource = newSource.release();
            }
        }
    }
    
    void playButtonClicked()
    {
        updateLoopState (loopingToggle.getToggleState());
        changeState (Starting);
    }
    
    void stopButtonClicked()
    {
        changeState (Stopping);
    }
    
    void loopButtonChanged()
    {
        updateLoopState (loopingToggle.getToggleState());
    }
    
    //==========================================================================
    TextButton openButton;
    TextButton playButton;
    TextButton stopButton;
    ToggleButton loopingToggle;
    Label currentPositionLabel;
    
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    TransportState state;

    LookAndFeel_V3 lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
