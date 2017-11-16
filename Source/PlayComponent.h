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
#include "ToggleSpaceComponent.h"
#include "Envelope.h"

//==============================================================================
/*
*/
class PlayComponent    : public Component
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
    Envelope env;
    
private:
    float tapDetectCoords [2][2];
    float coordinates [20][2];
    int coordIndex = 0;
    bool swipeEnd = false;
    ToggleSpaceComponent togSpaceComp;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayComponent)
};
