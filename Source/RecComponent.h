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
                        public Button::Listener,
                        public ChangeListener,
                        private Timer
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
    void setReadIndex (int *index);
    void setSelector (int *selected);
    void startRecording();
    void stopRecording();
    
    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    bool isRecording;
    bool recDone;
    
    bool getComponentSelected();
    void setComponentSelected(bool selected);
    
    int getComponentID();
    void setComponentID(int ID);
    
    void setPlayIndicatorVisible(bool state);
    
    void setSampleRate(float sR);
    bool isBufferEmpty();
    
    AudioThumbnail& getAudioThumbnail();
private:
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    bool displayFullThumb;
    bool displayPlaybackIndicator;
    
    float tapLimit = 0.1f;
    float time = 0.0f;
    bool tap = true;
    
    bool componentSelected = false;
    
    void timerCallback() override;
    
    int recID;
    float sampleRate;
    bool bufferEmpty = true;
    
    TextButton recordButton;
    AudioRecorder *recorder;
    int *readIndex;
    int *selected;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecComponent)
};
