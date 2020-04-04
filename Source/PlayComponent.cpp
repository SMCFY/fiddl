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
  sustainBackgroundImage(ImageFileFormat::loadFrom(BinaryData::sustainedbackdrop_png, (size_t) BinaryData::sustainedbackdrop_pngSize)),
  // load button icon images
  impulseButtonIconImage(ImageFileFormat::loadFrom(BinaryData::drum_icon_png, (size_t) BinaryData::drum_icon_pngSize)),
  sustainButtonIconImage(ImageFileFormat::loadFrom(BinaryData::trumpet_icon_png, (size_t) BinaryData::trumpet_icon_pngSize)),
  discreteButtonIconImage(ImageFileFormat::loadFrom(BinaryData::discretetoggle_png, (size_t) BinaryData::discretetoggle_pngSize)),
  loopButtonIconImage(ImageFileFormat::loadFrom(BinaryData::loopButtonIconImage_png, (size_t) BinaryData::loopButtonIconImage_pngSize))
{

    isPlaying = false;
    Gesture::setCompWidth(getWidth());
    Gesture::setCompHeight(getHeight());
    
    //ToggleSpace buttons
    addAndMakeVisible (toggleSustain);
    toggleSustain.setImages(true, true, true,
                            sustainButtonIconImage, 0.2f, Colours::transparentBlack,
                            sustainButtonIconImage, 0.8f, Colours::transparentBlack,
                            sustainButtonIconImage, 1.0f, Colours::transparentBlack);    toggleSustain.setClickingTogglesState(true);
    toggleSustain.setToggleState(true, dontSendNotification);
    toggleSustain.addListener (this);
    
    addAndMakeVisible (toggleImpulse);
    toggleImpulse.setImages(true, true, true,
                            impulseButtonIconImage, 0.2f, Colours::transparentBlack,
                            impulseButtonIconImage, 0.8f, Colours::transparentBlack,
                            impulseButtonIconImage, 1.0f, Colours::transparentBlack);    toggleSustain.setClickingTogglesState(true);
    toggleImpulse.setClickingTogglesState(true);
    toggleImpulse.addListener (this);
    
    //Discrete toggle button
    addAndMakeVisible (toggleDiscrete);
    toggleDiscrete.setImages(true, true, true,
                            discreteButtonIconImage, 0.2f, Colours::transparentBlack,
                            discreteButtonIconImage, 0.8f, Colours::transparentBlack,
                            discreteButtonIconImage, 1.0f, Colours::transparentBlack);    toggleDiscrete.setClickingTogglesState(true);
    toggleDiscrete.setClickingTogglesState(true);
    toggleDiscrete.addListener (this);

    //Loop toggle button
    addAndMakeVisible(toggleLoop);
    toggleLoop.setImages(true, true, true,
                        loopButtonIconImage, 0.2f, Colours::transparentBlack,
                        loopButtonIconImage, 0.8f, Colours::transparentBlack,
                        loopButtonIconImage, 1.0f, Colours::transparentBlack);    toggleLoop.setClickingTogglesState(true);
    toggleLoop.setClickingTogglesState(true);
    toggleLoop.addListener (this);

    ar = Envelope(Envelope::AR);
    adsr = Envelope(Envelope::ADSR);
}

PlayComponent::~PlayComponent()
{
}

void PlayComponent::paint (Graphics& g)
{
    g.fillAll(Colour().fromRGB(18, 21, 36)); // background color

    if(toggleSpaceID == 1) //graphics for sustained space
    {
        //g.setOpacity(0.05f);
        //g.drawImageWithin(sustainBackgroundImage, -100, 0, getWidth()+200, getHeight()+200, RectanglePlacement::centred); //set backdrop for sustained space
        g.setColour(Colour().fromRGB(120, 206, 214)); // render color

        if(pathEnabled && Gesture::getNumFingers() == 1) // draw path
        {           
            //for (int i = 0; i < Gesture::getNumFingers(); i++)
            //{
            Gesture::drawPath(g, Gesture::getPath(Gesture::getNumFingers()-1), Gesture::getNumFingers()-1);
            //}     
        }

        if(discretePitchToggled)
        {
            drawPitchBackDrop(g); //draws Pitchbar
        }
        else if(Gesture::getNumFingers() >= 2) // draw pinch indicator
        {
            drawPinchIndicator(g);
        }
    }
    else if (toggleSpaceID == 2) //graphics for impulse space
    {
        //g.setOpacity(0.05f);
        //g.drawImageWithin(impulseBackgroundImage, -200, -200, getWidth()+400, getHeight()+400, RectanglePlacement::centred); //set backdrop for impulse space
        drawImpulseBackdrop(g);
        toggleDiscrete.setToggleState(false, dontSendNotification);
        toggleLoop.setToggleState(false, dontSendNotification);
        discretePitchToggled = false;
        
        g.setColour(Colour().fromRGB(120, 206, 214)); // render color
        drawRipples(g);
    }
}

void PlayComponent::resized()
{
    int f = 7;
    Gesture::setCompWidth(getWidth());
    Gesture::setCompHeight(getHeight());
    toggleSustain.setBounds(getWidth()-(getWidth()/f + 5), 5, getWidth()/f, getWidth()/f);
    toggleImpulse.setBounds(getWidth()-(getWidth()/f + 5), getWidth()/f + 5, getWidth()/f, getWidth()/f);
    toggleDiscrete.setBounds(3,5,getWidth()/8,getWidth()/8);
    toggleLoop.setBounds(getWidth()-getWidth()/8-5, getHeight()-getHeight()/8, getWidth()/8,getWidth()/8);
}

void PlayComponent::mouseDown (const MouseEvent& e)
{
    Gesture::addFinger(e);
    mouseDrag(e);
    startTimer(60);

    if(getToggleSpaceID() == 1) // note on
    {
        adsr.trigger(1);
    }
    if(getToggleSpaceID() == 2)
    {
        ar.trigger(1);
        addRipple();
    }
      
    tapDetectCoords[0][0] = Gesture::getFingerPosition(0).x;
    tapDetectCoords[0][1] = Gesture::getFingerPosition(0).y;
    
    Mapper::setToggleSpace(toggleSpaceID);
    
    if(toggleLoop.getToggleState()==1) //looping is disabled on mouseUp, so we enable it again here IF the button is toggled
    {
        loopToggled = true;
    }
}

void PlayComponent::mouseDrag (const MouseEvent& e)
{
    Gesture::updateFingers(e.source, e.source.getIndex());
  
    Gesture::setVelocity(Gesture::getFingerPosition(0).x, Gesture::getFingerPosition(0).y);
    
    if(Gesture::getNumFingers() > 1 )
        Gesture::setDistBetweenFingers(Gesture::getNumFingers()-1);
    else
        Gesture::resetDistBetweenFingers();
        
    Gesture::setAbsDistFromOrigin(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x, Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
    
    if((discretePitchToggled && toggleSpaceID == 1))
    {
        //Mapper::routeParameters(Gesture::getNumFingers(),true); //before
        Mapper::routeParameters(1,true); //now hardcoded to 1 finger
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
    
    rectNum = Gesture::getPitchIndex();
    //repaint();
}

void PlayComponent::mouseUp (const MouseEvent& e)
{
    Gesture::setVelocityMax(Gesture::getVelocity());
    
    Gesture::rmFinger(e);

    Gesture::setResetPos(swipeEnd);
    
    Gesture::setTap(tapDetectCoords);

    rectNum = rectListSize;
    swipeEnd = true; // swipeEnd is a condition for resetting the buffer index when a new swipe is initiated
    
    loopToggled = false; //disable looping on mouseUp to avoid echo effect on release

    if(Gesture::getNumFingers() == 1)
    {
        Gesture::resetDistBetweenFingers();
    }

    if(toggleSpaceID == 1 && Gesture::getNumFingers() == 0) // note off (initiate release) 
    {
        pathEnabled = false;
        stopTimer();
        adsr.trigger(0);
    }
    
    repaint();
    recComp->repaint();
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
        toggleDiscrete.setVisible(false);
        toggleLoop.setVisible(false);

        discretePitchToggled = false;
        loopToggled = false;
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
        toggleDiscrete.setVisible(true);
        toggleLoop.setVisible(true);

        discretePitchToggled = false;
        loopToggled = false;
    }
    
    if(button == &toggleDiscrete)
    {
        if(toggleDiscrete.getToggleState()==0)
        {
            discretePitchToggled = false;
        }
        else
        {
            discretePitchToggled = true;
        }
    }

    if(button == &toggleLoop)
    {
        if(toggleLoop.getToggleState()==0)
        {
            loopToggled = false;
        }
        else
        {
            loopToggled = true;
        }
    }

    repaint();
    recComp->repaint();
}

int PlayComponent::getToggleSpaceID()
{
    return toggleSpaceID;
}

bool PlayComponent::getLoopState()
{
    return loopToggled;
}

bool PlayComponent::getDiscreteState()
{
    return discretePitchToggled;
}

void PlayComponent::timerCallback()
{
    if(toggleSpaceID == 1) // sustain
    {
        /*
        velocityRolloff *= 0.9;
        Gesture::setVelocity(velocityRolloff);
        
        if(velocityRolloff <= 0.03)
        {
            stopTimer();
        }
        */
        if(Gesture::getNumFingers() == 1)
            pathEnabled = true;
    }
    else // impulse
    {
        if(ripples.size() != 0)
        {
            for (int i = 0; i < ripples.size(); ++i)
            {
                if (ripples[i]->alpha < 0.1)
                    rmRipple(i);
            }
        }
        else
        {
            stopTimer();
        }
    }
    
    //NEED TO UPDATE PARAMETERS HERE FOR THE ROLLOFF TO AFFECT THE MAPPING
    //HOWEVER! If updateParameters() is called in a state where POSITION is mapped to something - CRASH
    //Mapper::routeParameters(0,false);
    //Mapper::updateParameters();

    repaint();
    recComp->repaint();
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

void PlayComponent::drawPitchBackDrop(Graphics& g)
{
    // refresh keyboard layout
    rectListBackDrop.clear();
    
    for (int i = 0; i < rectListSize; i++)
    {
        rectListBackDrop.add(Rectangle<float>(-5,(getHeight()/rectListSize+0.5)*i,getWidth()+5,getHeight()/rectListSize+0.5));
    }
    
    for (int i = 0; i < rectListSize; i++)
    {
        if(i%2 == 0)
        {
            //g.setColour (Colours::darkgrey);
            //g.setOpacity(0.1);
            
            g.setColour(Colour().fromRGB(18+1, 21+1, 36+1));
            g.fillRect(rectListBackDrop.getRectangle(i));
        }
        else
        {
            //g.setColour (Colours::darkgrey);
            //g.setOpacity(0.1);
            g.setColour(Colour().fromRGB(18+1, 21+1, 36+1));
            g.fillRect(rectListBackDrop.getRectangle(i));
        }
        
        g.setColour (Colours::white);
        g.setOpacity(0.1);
        g.drawRect(rectListBackDrop.getRectangle(i));
    }
    
    if(Gesture::getNumFingers() != 0)
    {
        if(discretePitchToggled)
        {
            //g.setColour (Colours::lightgrey);
            g.setColour(Colour().fromRGB(120, 206, 214));
            g.setOpacity(0.5);
            g.fillRect(rectListBackDrop.getRectangle(rectListSize-1-rectNum));
            g.setColour (Colours::white);
            g.setOpacity(0.5);
            g.drawRect(rectListBackDrop.getRectangle(rectListSize-1-rectNum));
        }
    }
}

void PlayComponent::addRipple()
{
    int lastFingerIndex = Gesture::getNumFingers()-1;
    PlayComponent::Ripple* rip = new PlayComponent::Ripple(Gesture::getFingerPositionScreen(lastFingerIndex), ripples.size());
    ripples.add(rip);
    startTimerHz(25);
}

void PlayComponent::rmRipple(int i)
{
    ripples.removeObject(ripples[i]);
}

void PlayComponent::drawRipples(Graphics& g)
{
    if(ripples.size() != 0)
    {
        for (int i = 0; i < ripples.size(); ++i) // fingers
        {
            if(ripples[i]->alpha < 0) //make sure that the alpha value never goes below 0
            {
                ripples[i]->alpha = 0.05;
            }
            
            g.setOpacity(ripples[i]->alpha);
            g.drawEllipse(ripples[i]->pos.x-ripples[i]->circleSize/2, ripples[i]->pos.y-ripples[i]->circleSize/2, ripples[i]->circleSize, ripples[i]->circleSize, ripples[i]->line);
            ripples[i]->acc = 30 * (1-Gesture::getAbsDistFromOrigin());
    
            ripples[i]->circleSize += ripples[i]->acc*=0.98; // increase circle radii
            ripples[i]->alpha -= 0.05; // 0.05 + ar.getAmplitude()*0.9; // decrease opacity
            if(ripples[i]->line > 0.2)
                ripples[i]->line -= 0.4; // decrease line thickness
            else
                ripples[i]->line = 0;  
        }
    }
}

void PlayComponent::drawPinchIndicator(Graphics& g)
{

    Point<float> f1(Gesture::getFingerPositionScreen(0).x, Gesture::getFingerPositionScreen(0).y);
    Point<float> f2(Gesture::getFingerPositionScreen(Gesture::getNumFingers()-1).x, Gesture::getFingerPositionScreen(Gesture::getNumFingers()-1).y);

    // touch indicators
    g.setOpacity(1.0f);
    g.drawEllipse(f1.x-(tipSize/2), f1.y-(tipSize/2), tipSize, tipSize, tipThickness);
    g.drawEllipse(f2.x-(tipSize/2), f2.y-(tipSize/2), tipSize, tipSize, tipThickness);

    // diameter

    // pinch circle
    g.setOpacity(0.5f);
    pinchSize = std::sqrt(std::pow(f2.x-f1.x, 2)+std::pow(f2.y-f1.y, 2));
    Point<float> centerPosition(jmin(f1.x, f2.x) + (jmax(f1.x, f2.x)-jmin(f1.x, f2.x))/2, jmin(f1.y, f2.y) + (jmax(f1.y, f2.y)-jmin(f1.y, f2.y))/2);

    g.drawEllipse(centerPosition.x-(pinchSize/2), centerPosition.y-(pinchSize/2), pinchSize, pinchSize, pinchThickness);

}

bool PlayComponent::isPlaying;
bool PlayComponent::initRead;

void PlayComponent::startPlaying()
{
    initRead = true; // reset readIndex
    isPlaying = true;
}

void PlayComponent::stopPlaying()
{
    initRead = true; // reset readIndex
    isPlaying = false;
    //std::cout << "STOPPALYIN " << std::endl;
}

void PlayComponent::drawImpulseBackdrop(Graphics& g)
{
    for (int i = 4; i > 0; i--)
    {
        g.setColour(Colour().fromRGB(18+i, 21+i, 36+i));
        g.fillEllipse(getWidth()/2 - (i*getHeight()/4)/2, getHeight()/2-(i*getHeight()/4)/2, i*getHeight()/4,i*getHeight()/4);
        
        //Add stroke
        //g.setColour(Colours::white);
        //g.setOpacity(0.02);
        //g.drawEllipse(getWidth()/2 - (i*getHeight()/4)/2, getHeight()/2-(i*getHeight()/4)/2, i*getHeight()/4,i*getHeight()/4,3);
    }
}

void PlayComponent::setRecComp(RecComponent *recComp)
{
    this->recComp = recComp;
}
