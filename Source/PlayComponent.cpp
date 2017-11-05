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
#include <iostream>
#include <string>

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
        g.drawEllipse(int (x * getWidth() - 15), int (getHeight() - (y * getHeight()) - 15), 30*Gesture::getVelocity(), 30*Gesture::getVelocity(), 2);
    }
    
    
}

void PlayComponent::resized()
{
}

void PlayComponent::mouseDown (const MouseEvent& e)
{
  Gesture::addFinger(e);
  startPlaying();
  mouseDrag (e);
}

void PlayComponent::mouseDrag (const MouseEvent& e)
{
  // retrieve the x position, from 0.0 to 1.0
  x = e.position.x / getWidth();

  // retrieve the y position, from 0.0 to 1.0
  y = ((getHeight() - e.position.y) > 0 ? (getHeight() - e.position.y) / getHeight() : 0);
                      
  
  Gesture::updateFingers(e);

  Gesture::setVelocity(x,y);
    
  Mapper::updateParameters();
    
  //Fill the buffer for calculating direction, and calculate direction when the buffer reaches the end
    if (coordIndex > Gesture::directionBuffSize - 1)
  {
      Gesture::setDirection(coordinates);
      coordIndex = 0;
      //std::cout << "Get Direction!" ;
      
  } else if (swipeEnd)
  {
      //Start writing to the first index again and set all indexes equal to the first, to make sure that the deltaPosition is 0
      coordIndex = 0;
      
      for(int i = 0; i < Gesture::directionBuffSize; i++)
      {
          coordinates[i][0] = x;
          coordinates[i][1] = y;
      }
      
      swipeEnd = false;
      //std::cout << "New Swipe!" ;
  }
  else
  {
      coordinates[coordIndex][0] = x;
      coordinates[coordIndex][1] = y;
      coordIndex++;
  }

  repaint();
}

void PlayComponent::mouseUp (const MouseEvent& e)
{
  stopPlaying();
  
  //swipeEnd is a condition for resetting the buffer index when a new swipe is initiated
  swipeEnd = true;
  Gesture::setResetPos(swipeEnd);
  
  //***Buggy. Returns wrong direction on release***
  coordinates[Gesture::directionBuffSize-1][0] = coordinates[coordIndex-1][0];
  coordinates[Gesture::directionBuffSize-1][1] = coordinates[coordIndex-1][1];;
  Gesture::setDirection(coordinates);
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

