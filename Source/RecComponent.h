/*
  ==============================================================================

    RecComponent.h
    Created: 11 Oct 2017 1:06:39pm
    Authors: Jonas Holfelt
             Gergely Csapo
             Michael Castanieto

    Description:  GUI component for recording audio. Recording is triggered when
                  recordButton is held down. Recording stops when recordButton is
                  released or when the recording buffer has reached maximum
                  capacity.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorder.h"

//==============================================================================
/*
*/
class RecComponent    : public Component,
                        public Button::Listener
{
public:
    RecComponent();
    ~RecComponent();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* button) override;
    void buttonStateChanged (Button* button) override;
    /* setRecorder uses an instance of AudioRecorder in order to allow the GUI 
       component to record audio from the device's microphone to a buffer */
    void setRecorder (AudioRecorder *recorder);
    void startRecording();
    void stopRecording();

    bool isRecording;
    bool recDone; 
private:
    TextButton recordButton;
    AudioRecorder *recorder;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecComponent)
};
