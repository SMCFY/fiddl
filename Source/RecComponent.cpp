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
    const int width = getWidth();
    float sampLength = recorder->getSampLength();
    if (sampLength == 0)
        sampLength = 44100*3;
    float index = *readIndex * width / sampLength;
    g.fillAll(Colour().fromRGB(18, 21, 36)); // background color
    g.setColour(Colours::lightgrey);
    
    if (thumbnail.getTotalLength() > 0.0)
    {
        Rectangle<int> thumbArea (getLocalBounds());
        if (recDone)
        {
            g.fillAll (Colours::orangered);
            thumbnail.drawChannels(g, thumbArea.reduced(1), 0.0f, 3.0f, 1.0f);
           
        }
        else
        {
            g.fillAll(Colour().fromRGB(18, 21, 36)); // background color
            thumbnail.drawChannels(g, thumbArea.reduced(2), 0.0f, sampLength/(float)recorder->getSampleRate(), 1.0f);
        }
    }
    else
    {
        g.setFont(14.0f);
        g.drawFittedText("Hold to Record", getLocalBounds(), Justification::centred, 2);
    }
    g.setColour (Colours::darkgrey); // border color
    g.drawLine(0, 0, getWidth(), 0, 1);
    g.setColour(Colours::green);
    g.drawLine(index, 0.0f, index, getHeight(), 1.0f);
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

void RecComponent::setReadIndex(int *readIndex)
{
    this->readIndex = readIndex;
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
