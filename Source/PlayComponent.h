/*
  ==============================================================================

    PlayComponent.h
    Created: 11 Oct 2017 1:07:24pm
    Author:  Michael Castanieto

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
    void resized() override;
    
    // public variables
    bool isPlaying; // true if recorded audio is playing

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayComponent)
};
