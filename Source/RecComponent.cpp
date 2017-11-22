/*
  ==============================================================================

    RecComponent.cpp
    Created: 11 Oct 2017 1:06:39pm
    Authors: Jonas Holfelt
             Gergely Csapo
             Michael Castanieto

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RecComponent.h"

//==============================================================================
RecComponent::RecComponent()
{
    addAndMakeVisible (recordButton);
    recordButton.setButtonText ("Hold to record");
    recordButton.addListener (this);
    isRecording = false;
    recDone = false;

}

RecComponent::~RecComponent()
{
}

void RecComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
}

void RecComponent::resized()
{
    recordButton.setBounds (getWidth() / 2 - (getWidth() / 3 / 2), getHeight() / 2 - (getHeight() / 4), getWidth() / 3, getHeight() / 2);
}

void RecComponent::buttonClicked (Button* button)
{
}

void RecComponent::buttonStateChanged (Button* button)
{
    if (recordButton.isDown())
    {
        startRecording();
        recDone = true;
    }
    else if (recordButton.isOver() && recDone)
    {
        stopRecording();
        recDone = false;
    }
}

void RecComponent::setRecorder (AudioRecorder *recorder)
{
    this->recorder = recorder;
}

void RecComponent::startRecording()
{
    recorder->startRecording();
    recordButton.setButtonText ("Recording");
}

void RecComponent::stopRecording()
{
    recorder->stop();
    recordButton.setButtonText ("Hold to Record");
}
