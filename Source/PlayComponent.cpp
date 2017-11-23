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
    
    env = Envelope(44100);
    env.isTriggered = &isPlaying;
    
    //ToggleSpace buttons
    addAndMakeVisible (toggleSustain);
    toggleSustain.setButtonText ("Sustain");
    toggleSustain.setClickingTogglesState(true);
    toggleSustain.setToggleState(true, dontSendNotification);
    toggleSustain.addListener (this);
    
    addAndMakeVisible (toggleImpulse);
    toggleImpulse.setButtonText ("Impulse");
    toggleImpulse.setClickingTogglesState(true);
    toggleImpulse.addListener (this);
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
    
    ////Draw discrete pitch bar if sustain mode is picked
    if(toggleSpaceID == 1)
    {
        for (int i = 0; i < 12; i++)
        {
            rectList.add(Rectangle<float>((getWidth()/12+0.5)*i,getHeight()-75,getWidth()/12+0.5,75));
        }
        
        for (int i = 0; i < 12; i++)
        {
            if(i%2 == 0)
            {
                g.setColour (Colours::darkgrey);
                g.setOpacity(0.3);
                g.fillRect(rectList.getRectangle(i));
            }
            else
            {
                g.setColour (Colours::darkgrey);
                g.setOpacity(0.3);
                g.fillRect(rectList.getRectangle(i));
            }
            
            g.setColour (Colours::white);
            g.setOpacity(0.3);
            g.drawRect(rectList.getRectangle(i));
        }
        
        if(Gesture::getNumFingers() != 0)
        {
            if(Gesture::getFingerPosition(0).y >= getHeight()*Gesture::getFingerPosition(0).y - 75)
            {
                g.setColour (Colours::darkgrey);
                g.setOpacity(1.0);
                g.fillRect(rectList.getRectangle(rectNum));
                g.setColour (Colours::white);
                g.drawRect(rectList.getRectangle(rectNum));
            }
        }
    }
    
    if(Gesture::getNumFingers() != 0)
    {
      for (int i = 0; i < Gesture::getNumFingers(); i++)
      {
        g.setOpacity(1.0);
        g.drawEllipse(int (Gesture::getFingerPosition(i).x * getWidth() - 15), int (getHeight() - (Gesture::getFingerPosition(i).y * getHeight()) - 15), 50*Gesture::getVelocity(), 50*Gesture::getVelocity(), 2);
      }
    }
}

void PlayComponent::resized()
{
    Gesture::setCompWidth(getWidth());
    Gesture::setCompHeight(getHeight());
    toggleSustain.setBounds(getWidth()-82, 5, 80,30);
    toggleImpulse.setBounds(getWidth()-82, 35, 80,30);
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
    
    Mapper::setToggleSpace(toggleSpaceID);
}

void PlayComponent::mouseDrag (const MouseEvent& e)
{                      
    Gesture::updateFingers(e.source, e.source.getIndex());
  
    Gesture::setVelocity(Gesture::getFingerPosition(0).x, Gesture::getFingerPosition(0).y);
    Gesture::setAbsDistFromOrigin(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x, Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
    
    if(Gesture::getFingerPosition(0).y >= getHeight()*Gesture::getFingerPosition(0).y - 75 && toggleSpaceID == 1)
    {
        Mapper::routeParameters(Gesture::getNumFingers(),true);
        Mapper::updateParameters();
    }
    else
    {
        Mapper::routeParameters(Gesture::getNumFingers(),false);
        Mapper::updateParameters();
    }
    
    fillCoordinates();
    tapDetectCoords[1][0] = Gesture::getFingerPosition(0).x;
    tapDetectCoords[1][1] = Gesture::getFingerPosition(0).y;
    
    rectNum = int(Gesture::getDiscretePitch()/2+6);
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

    rectNum = 12;
    repaint();
}

void PlayComponent::buttonClicked (Button* button)
{
    if(button == &toggleImpulse)
    {
        if(toggleImpulse.getToggleState()==0)
        {
            toggleImpulse.setToggleState(true, dontSendNotification);
        }
        else
        {
            toggleSustain.setToggleState(false, dontSendNotification);
            toggleSpaceID = 2;
        }
    }
    
    if(button == &toggleSustain)
    {
        if(toggleSustain.getToggleState()==0)
        {
            toggleSustain.setToggleState(true,dontSendNotification);
        }
        else
        {
            toggleImpulse.setToggleState(false, dontSendNotification);
            toggleSpaceID = 1;
        }
    }
    repaint();
}

int PlayComponent::getToggleSpaceID()
{
    return toggleSpaceID;
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
