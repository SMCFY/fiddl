/*
  ==============================================================================

    PlayComponent.cpp
    Created: 11 Oct 2017 1:07:24pm
    Authors:  Michael Castanieto
              Jonas Holfelt
              Gergely Csapo
              
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayComponent.h"

//==============================================================================
PlayComponent::PlayComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    isPlaying = false;

}

PlayComponent::~PlayComponent()
{
}

void PlayComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlayComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void PlayComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void PlayComponent::stopPlaying()
{
    isPlaying = false;
}

void PlayComponent::startPlaying()
{
    isPlaying = true;
}