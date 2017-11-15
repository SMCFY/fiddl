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
class ToggleSpaceComponent    : public Component,
public Button::Listener
{
public:
    ToggleSpaceComponent();
    ~ToggleSpaceComponent();
    
    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* button) override;
    void buttonStateChanged (Button* button) override;
    int getToggleSpace();
    void setToggleSpace(int t);
    
    

private:
    TextButton toggleSustain;
    TextButton toggleImpulse;
    int toggleSpace = 1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToggleSpaceComponent)
};
