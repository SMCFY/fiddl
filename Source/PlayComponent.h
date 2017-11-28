/*
  ==============================================================================

    PlayComponent.h
    Created: 11 Oct 2017 1:07:24pm
    Authors: Michael Castanieto
             Jonas Holfelt
             Gergely Csapo

    Description:  GUI component for playing audio. Playing is triggered when
                  the play component is held down. Playing stops when the 
                  component is released or when playback has reached the end
                  of the recorded segment in the buffer. Passes mouse coordinates
                  to Gesture class.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "ToggleSpaceComponent.h"
#include "Envelope.h"

//==============================================================================
/*
*/
class PlayComponent    : public Component,
public Button::Listener,
private Timer
{
public:
    PlayComponent();
    ~PlayComponent();

    void paint (Graphics&) override;
    void resized() override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    
    void fillCoordinates();

    void stopPlaying(); // stop audio playback
    void startPlaying(); // start audio playback
    
    bool isPlaying; // true if recorded audio is playing
    bool initRead; // resets readIndex in MCC
    Envelope env;
  //  ToggleSpaceComponent togSpaceComp;
    
    //for ToggleSpace buttons
    int getToggleSpaceID();
    void buttonClicked (Button* button) override;
    int getToggleSpace();
    void setToggleSpace(int id);
    
    //velocity rolloff
    float velocityRolloff;
    void startRolloff();
    
private:
    //Detect tap and/or direction of swipe
    float tapDetectCoords [2][2];
    float coordinates [20][2];
    int coordIndex = 0;
    bool swipeEnd = false;
    
    //Rectangle array for the discrete pitch bar
    RectangleList<float> rectList;
    int rectNum = 12;
    
    //Togglespace buttons
    TextButton toggleSustain;
    TextButton toggleImpulse;
    int toggleSpaceID = 1;
    
    //GUI effects
    void drawPitchBar(Graphics& g);
    void timerCallback() override;
    void startRipple();
    void fillRippleCoords();
    void drawRipples(Graphics& g);
    float circleAlpha = 0.0f;
    float circleSize [3] = {20,20,20};
    float circleRippleSpeed [3] = {1.15,1.2,1.25};
    float rippleCoords [10][2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayComponent)
};
