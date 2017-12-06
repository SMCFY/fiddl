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
RecComponent::RecComponent() : thumbnailCache(10),
                               thumbnail (512, formatManager, thumbnailCache),
                               displayFullThumb(false)
{
    //addAndMakeVisible (recordButton);
    //recordButton.setButtonText ("Hold to record");
    recordButton.addListener (this);
    isRecording = false;
    recDone = false;
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);

}

RecComponent::~RecComponent()
{
    thumbnail.removeChangeListener(this);
}

void RecComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    g.setColour(Colours::lightgrey);
    
    if (thumbnail.getTotalLength() > 0.0)
    {
        Rectangle<int> thumbArea (getLocalBounds());
        if (recDone)
        {
            g.fillAll (Colours::red);
            thumbnail.drawChannels(g, thumbArea.reduced(1), 0.0f, 3.0f, 1.0f);
           
        }
        else
        {
            g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
            thumbnail.drawChannels(g, thumbArea.reduced(2), 0.0f, recorder->getSampLength()/44100.f, 1.0f);
        }
    }
    else
    {
        g.setFont(14.0f);
        g.drawFittedText("Hold to Record", getLocalBounds(), Justification::centred, 2);
    }
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
    else if (!recordButton.isDown() && recDone) //ƒiddl
    {
        stopRecording();
        recDone = false;
    }
}

void RecComponent::mouseDown(const MouseEvent& event)
{
    startRecording();
    recDone = true;
}

void RecComponent::mouseUp(const MouseEvent& event)
{
    if (recDone)
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
    //recordButton.setButtonText ("Recording");
}

void RecComponent::stopRecording()
{
    recorder->stop();
    repaint();
    //recordButton.setButtonText ("Hold to Record");
}

void RecComponent::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &thumbnail)
        repaint();
}

AudioThumbnail& RecComponent::getAudioThumbnail()
{
    return thumbnail;
}
