/*
  ==============================================================================

    PlayComponent.h
    Created: 11 Oct 2017 1:07:24pm
    Authors:  Michael Castanieto
              Jonas Holfelt
              Gergely Csapo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PlayComponent    : public Component
{
public:
    // constructor/destructor
    PlayComponent();
    ~PlayComponent();

    // public methods inherited from Component
    void paint (Graphics&) override;
    void resized () override;
    // mouse event handlers 
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;

    void stopPlaying(); // stop audio playback
    void startPlaying(); // start audio playback
    
    // public variables
    bool isPlaying; // true if recorded audio is playing
    float x; // x position for tracking a finger press
    float y; // y position for tracking a finger press

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayComponent)
};
