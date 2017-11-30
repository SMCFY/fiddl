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
#include "AudioProcessorBundler.h"
#include <iostream>
#include <string>

//==============================================================================
PlayComponent::PlayComponent()
  // load background images
: impulseBackgroundImage(ImageFileFormat::loadFrom(BinaryData::drumbackdrop_png, (size_t) BinaryData::drumbackdrop_pngSize)),
  sustainBackgroundImage(ImageFileFormat::loadFrom(BinaryData::kid_jpg, (size_t) BinaryData::kid_jpgSize)),
  // load button icon images
  impulseButtonIconImage(ImageFileFormat::loadFrom(BinaryData::drum_icon_png, (size_t) BinaryData::drum_icon_pngSize)),
  sustainButtonIconImage(ImageFileFormat::loadFrom(BinaryData::trumpet_icon_png, (size_t) BinaryData::trumpet_icon_pngSize))
{

    isPlaying = false;
    Gesture::setCompWidth(getWidth());
    Gesture::setCompHeight(getHeight());
    
    //ToggleSpace buttons
    addAndMakeVisible (toggleSustain);
    //toggleSustain.setButtonText ("Sustain");
    toggleSustain.setImages(true, true, true,
                            sustainButtonIconImage, 0.5f, Colours::transparentBlack,
                            sustainButtonIconImage, 0.8f, Colours::transparentBlack,
                            sustainButtonIconImage, 1.0f, Colours::transparentBlack);    toggleSustain.setClickingTogglesState(true);
    toggleSustain.setToggleState(true, dontSendNotification);
    toggleSustain.addListener (this);
    
    addAndMakeVisible (toggleImpulse);
    //toggleImpulse.setButtonText ("Impulse");
    toggleImpulse.setImages(true, true, true,
                            impulseButtonIconImage, 0.5f, Colours::transparentBlack,
                            impulseButtonIconImage, 0.8f, Colours::transparentBlack,
                            impulseButtonIconImage, 1.0f, Colours::transparentBlack);    toggleSustain.setClickingTogglesState(true);
    toggleImpulse.setClickingTogglesState(true);
    toggleImpulse.addListener (this);

    AudioProcessorBundler::ar.isTriggered = &isPlaying;
    AudioProcessorBundler::adsr.isTriggered = &isPlaying;
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

    if(toggleSpaceID == 1) //graphics for sustained space
    {
        g.drawImageWithin(sustainBackgroundImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::onlyReduceInSize); //set backdrop for sustained space
        drawPitchBar(g); //draws Pitchbar
        
        if(Gesture::getNumFingers() != 0) //draw ellipse on the users finger positions
        {
            for (int i = 0; i < Gesture::getNumFingers(); i++)
            {
                g.setOpacity(1.0f);
                g.drawEllipse(int (Gesture::getFingerPosition(i).x * getWidth() - (50*(std::pow(Gesture::getVelocity()/2+1,4)))/2), int (getHeight() - (Gesture::getFingerPosition(i).y * getHeight()) - (50*(std::pow(Gesture::getVelocity()/2+1,4)))/2), 50*(std::pow(Gesture::getVelocity()/2+1,4)), 50*(std::pow(Gesture::getVelocity()/2+1,4)), 2);
                
                Gesture::drawPath(g, Gesture::getPath(i), i);
            }
        }
    }
    else if (toggleSpaceID == 2) //graphics for impulse space
    {
        g.drawImageWithin(impulseBackgroundImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::onlyReduceInSize); //set backdrop for impulse space
        drawRipples(g);
    }
}

void PlayComponent::resized()
{
    std::cout << getWidth() << "    ";
    std::cout << getHeight();
    Gesture::setCompWidth(getWidth());
    Gesture::setCompHeight(getHeight());
    toggleSustain.setBounds(getWidth()-72, 5, 70, 70);
    toggleImpulse.setBounds(getWidth()-112, 45, 150, 150);
}

void PlayComponent::mouseDown (const MouseEvent& e)
{
    Gesture::addFinger(e);
    mouseDrag (e);

    if(getToggleSpaceID() == 1) // note on
        AudioProcessorBundler::adsr.trigger(1);
    if(getToggleSpaceID() == 2)
        AudioProcessorBundler::ar.trigger(1);

    initRead = true; // reset readIndex
    isPlaying = true;
      
    tapDetectCoords[0][0] = Gesture::getFingerPosition(0).x;
    tapDetectCoords[0][1] = Gesture::getFingerPosition(0).y;
    
    fillRippleCoords();
    
    Mapper::setToggleSpace(toggleSpaceID);
    
    
    //Reset ripple values
    if(Gesture::getNumFingers() == 1)
    {
        for (int i = 1; i < 8; i++)
        {
            rippleCoords[i][0] = 0;
            rippleCoords[i][1] = 0;
        }
    }
    
    for(int i = 0; i < 3; i++)
    {
        circleSize[i] = 20;// + 10 *i;
    }
    circleAlpha = 1.0f;
    stopTimer();
    if(toggleSpaceID == 2)
        startRipple();
}

void PlayComponent::mouseDrag (const MouseEvent& e)
{                      
    Gesture::updateFingers(e.source, e.source.getIndex());
  
    Gesture::setVelocity(Gesture::getFingerPosition(0).x, Gesture::getFingerPosition(0).y);

    Gesture::setAbsDistFromOrigin(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x, Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
    
    if(Gesture::getFingerPosition(0).y >= getHeight()*Gesture::getFingerPosition(0).y - getHeight()/6 && toggleSpaceID == 1)
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
    Gesture::setVelocityMax(Gesture::getVelocity());
    
    if(toggleSpaceID == 1)
        startRolloff();
    
    Gesture::rmFinger(e);

    if(Gesture::getNumFingers() == 0) // note off (initiate release) 
    {
        AudioProcessorBundler::ar.trigger(0);
        AudioProcessorBundler::adsr.trigger(0);
    }
    
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

void PlayComponent::timerCallback()
{
    //std::cout << AudioProcessorBundler::adsr.getAmplitude() << "\n";
    if(toggleSpaceID == 1)
    {
        //Velocity rolloff
        velocityRolloff *= 0.9;
        Gesture::setVelocity(velocityRolloff);
        
        if(velocityRolloff <= 0.03)
        {
            stopTimer();
        }
    }
    
    else
    {
        circleAlpha = AudioProcessorBundler::ar.getAmplitude();
        
        for(int i = 0; i < 3; i++)
        {
            circleSize[i] *= circleRippleSpeed[i];
        }
        
        
        if(circleAlpha <= 0.1)
        {
            circleAlpha = 0.0f;
            stopTimer();
            repaint();
        }
    }
    //std::cout << Gesture::getVelocityMax() << "\n";
    
    //NEED TO UPDATE PARAMETERS HERE FOR THE ROLLOFF TO AFFECT THE MAPPING
    //HOWEVER! If updateParameters() is called in a state where POSITION is mapped to something - CRASH
    //Mapper::routeParameters(0,false);
    //Mapper::updateParameters();
    repaint();
}

void PlayComponent::startRipple()
{
    startTimerHz(30);
}

void PlayComponent::startRolloff()
{
    velocityRolloff = Gesture::getVelocity();
    startTimer(30);
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

void PlayComponent::fillRippleCoords()
{
    
    //Fill coordinates array for administrating the ripple effects
    for (int i = 0; i < Gesture::getNumFingers(); i++)
    {
        rippleCoords[i][0] = Gesture::getFingerPosition(i).x;
        rippleCoords[i][1] = Gesture::getFingerPosition(i).y;
    }
}

void PlayComponent::drawRipples(Graphics& g)
{
    g.setOpacity(circleAlpha); //draw ripples on users finger position
    for(int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            if(rippleCoords[k][0] == 0)
                g.setOpacity(0.0);
            else
            {
                g.setOpacity(circleAlpha);
                g.drawEllipse(int (rippleCoords[k][0] * getWidth() - circleSize[i]/2), int (getHeight() - (rippleCoords[k][1] * getHeight()) - circleSize[i]/2), circleSize[i], circleSize[i], 2);
            }
        }
    }
}

void PlayComponent::drawPitchBar(Graphics& g)
{
    for (int i = 0; i < 12; i++)
    {
        rectList.add(Rectangle<float>((getWidth()/12+0.5)*i,getHeight()-getHeight()/6,getWidth()/12+0.5,getHeight()/6));
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
        if(Gesture::getFingerPosition(0).y >= getHeight()*Gesture::getFingerPosition(0).y - getHeight()/6)
        {
            g.setColour (Colours::lightgrey);
            g.setOpacity(0.8);
            g.fillRect(rectList.getRectangle(rectNum));
            g.setColour (Colours::white);
            g.drawRect(rectList.getRectangle(rectNum));
        }
    }
}
