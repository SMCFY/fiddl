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
    if (isPlaying)
    {
        g.drawText ("Playing", getLocalBounds(),
                Justification::centred, true);
    }
    else
    {
        g.drawText ("Stopped", getLocalBounds(),
                Justification::centred, true);
    }
    
    //Draw a shape on mouseDrag
    if(isPlaying)
        g.drawEllipse(int(x*getWidth()-25), int(getHeight()-(y*getHeight())-25), 50, 50,2);
}

void PlayComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void PlayComponent::mouseDown (const MouseEvent& e)
{
  startPlaying();
  mouseDrag (e);
}

void PlayComponent::mouseDrag (const MouseEvent& e)
{
  // retrieve the x position, from 0.0 to 1.0
  x = e.position.x/getWidth();

  // retrieve the y position, from 0.0 to 1.0
  double height = getHeight();
  double yPos = height - e.position.y;
  y = (yPos > 0 ? yPos/height : 0);
  repaint();
}

void PlayComponent::mouseUp (const MouseEvent& e)
{
  stopPlaying();
}

void PlayComponent::stopPlaying()
{
    repaint();
    isPlaying = false;
}

void PlayComponent::startPlaying()
{
    repaint();
    isPlaying = true;
}
