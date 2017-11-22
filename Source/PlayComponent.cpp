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
    Gesture::setCompWidth(getWidth());
    Gesture::setCompHeight(getHeight());
    
    addAndMakeVisible (togSpaceComp);
    togSpaceComp.setSize (100, 100);
    
    env = Envelope(44100, Envelope::ar);
    env.isTriggered = &isPlaying;
    togSpaceComp.setToggleSpace(1);
}

PlayComponent::~PlayComponent()
{
}

void PlayComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);
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
    
    if(Gesture::getNumFingers() != 0)
    {
      for (int i = 0; i < Gesture::getNumFingers(); i++)
      {
        g.drawEllipse(int (Gesture::getFingerPosition(i).x * getWidth() - 15), int (getHeight() - (Gesture::getFingerPosition(i).y * getHeight()) - 15), 50*Gesture::getVelocity(), 50*Gesture::getVelocity(), 2);
      }
    }
    
    ////Draw discrete pitch bar if sustain mode is picked
    if(togSpaceComp.getToggleSpace() == 1)
    {
        for (int i = 0; i < 12; i++)
        {
            rectList.add(Rectangle<float>((getWidth()/12+0.5)*i,getHeight()-75,getWidth()/12+0.5,75));
        }
        
        for (int i = 0; i < 12; i++)
        {
            if(i%2 == 0)
            {
                g.setColour (Colours::grey);
                g.fillRect(rectList.getRectangle(i));
            }
            else
            {
                g.setColour (Colours::darkgrey);
                g.fillRect(rectList.getRectangle(i));
            }
            
            g.setColour (Colours::white);
            g.drawRect(rectList.getRectangle(i));
        }
    }
}

void PlayComponent::resized()
{
    Gesture::setCompWidth(getWidth());
    Gesture::setCompHeight(getHeight());
    togSpaceComp.setBounds(getWidth()-82,5,80,60);
}

void PlayComponent::mouseDown (const MouseEvent& e)
{
    Gesture::addFinger(e);

    env.trigger(1); // note on (initiate attack)
    initRead = true; // reset readIndex
    isPlaying = true;
    
    mouseDrag (e);
      
    tapDetectCoords[0][0] = Gesture::getFingerPosition(0).x;
    tapDetectCoords[0][1] = Gesture::getFingerPosition(0).y;
    
    Mapper::setToggleSpace(togSpaceComp.getToggleSpace());
}

void PlayComponent::mouseDrag (const MouseEvent& e)
{                      
    Gesture::updateFingers(e.source, e.source.getIndex());
  
    Gesture::setVelocity(Gesture::getFingerPosition(0).x, Gesture::getFingerPosition(0).y);
    Gesture::setAbsDistFromOrigin(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x, Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
    
    if(Gesture::getFingerPosition(0).y >= getHeight()*Gesture::getFingerPosition(0).y - 75 && togSpaceComp.getToggleSpace() == 1)
    {
        Mapper::routeParameters(5);
        Mapper::updateParameters();
    }
    else
    {
        Mapper::routeParameters(Gesture::getNumFingers());
        Mapper::updateParameters();
    }
    
    fillCoordinates();
    tapDetectCoords[1][0] = Gesture::getFingerPosition(0).x;
    tapDetectCoords[1][1] = Gesture::getFingerPosition(0).y;
      
    repaint();
}

void PlayComponent::mouseUp (const MouseEvent& e)
{
    Gesture::rmFinger(e);

    if(Gesture::getNumFingers() == 0)
        env.trigger(0); // note off (initiate release)  
    
    swipeEnd = true; // swipeEnd is a condition for resetting the buffer index when a new swipe is initiated
    Gesture::setResetPos(swipeEnd);
    
    Gesture::setTap(tapDetectCoords);

    repaint();
}

void PlayComponent::fillCoordinates()
{
    //Fill the buffer for calculating direction, and calculate direction when the buffer reaches the end
    if (coordIndex > Gesture::directionBuffSize - 1)
    {
        Gesture::setDirection(coordinates);
        coordIndex = 0;
    }
    
    else if (swipeEnd)
    {
        //Start writing to the first index again and set all indexes equal to the first, to make sure that the deltaPosition is 0
        coordIndex = 0;
        
        for(int i = 0; i < Gesture::directionBuffSize; i++)
        {
            coordinates[i][0] = Gesture::getFingerPosition(0).x;
            coordinates[i][1] = Gesture::getFingerPosition(0).y;
        }
        
        swipeEnd = false;
    }
    
    else
    {
        coordinates[coordIndex][0] = Gesture::getFingerPosition(0).x;
        coordinates[coordIndex][1] = Gesture::getFingerPosition(0).y;
        coordIndex++;
    }
}
