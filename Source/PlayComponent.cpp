/*
  ==============================================================================

    PlayComponent.cpp
    Created: 11 Oct 2017 1:07:24pm
    Authors: Michael Castanieto
             Jonas Holfelt
             Gergely Csapo
              
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayComponent.h"
#include "Gesture.h"
#include "Mapper.h"

//==============================================================================
PlayComponent::PlayComponent()
{
    isPlaying = false;
}

PlayComponent::~PlayComponent()
{
}

void PlayComponent::paint (Graphics& g)
{
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
    if(isPlaying){
        g.drawEllipse(int (x * getWidth() - 15), int (getHeight() - (y * getHeight()) - 15), 30*Gesture::getVelocityX(), 30*Gesture::getVelocityX(), 2);
    }
    
    
}

void PlayComponent::resized()
{
}

void PlayComponent::mouseDown (const MouseEvent& e)
{
  startPlaying();
  
  mouseDrag (e);
}

void PlayComponent::mouseDrag (const MouseEvent& e)
{
  // retrieve the x position, from 0.0 to 1.0
  x = e.position.x / getWidth();

  // retrieve the y position, from 0.0 to 1.0
  y = ((getHeight() - e.position.y) > 0 ? (getHeight() - e.position.y) / getHeight() : 0);

  Gesture::setXPos(x);
  Gesture::setYPos(y);
  Gesture::setVelocity(x,y);
    
  Mapper::updateParameters();
  
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
