/*
  ==============================================================================

    ToggleSpaceComponent.h
    Created: 10 Nov 2017 8:37:18pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ToggleSpaceComponent    : public Component
{
public:
    ToggleSpaceComponent();
    ~ToggleSpaceComponent();
    
    void paint (Graphics&) override;
    void resized() override;
    //void buttonStateChanged (Button* button) override;
    //void buttonClicked (Button* button) override;

private:
    ToggleButton toggleSustain;
    ToggleButton toggleImpulse;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToggleSpaceComponent)
};
