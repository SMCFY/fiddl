/*
  ==============================================================================

    ToggleSpaceComponent.cpp
    Created: 10 Nov 2017 8:37:18pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ToggleSpaceComponent.h"

//==============================================================================
ToggleSpaceComponent::ToggleSpaceComponent()
{
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

ToggleSpaceComponent::~ToggleSpaceComponent()
{
}

void ToggleSpaceComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void ToggleSpaceComponent::resized()
{
    toggleSustain.setBounds(0, 0, 80,30);
    toggleImpulse.setBounds(0, 30, 80,30);
}

void ToggleSpaceComponent::buttonClicked (Button* button)
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
            toggleSpace = 2;
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
            toggleSpace = 1;
        }
    }
}

void ToggleSpaceComponent::buttonStateChanged (Button* button)
{
    
}

int ToggleSpaceComponent::getToggleSpace()
{
    std::cout << toggleSpace;
    return toggleSpace;
}

void ToggleSpaceComponent::setToggleSpace(int t)
{
    toggleSpace = t;
}
