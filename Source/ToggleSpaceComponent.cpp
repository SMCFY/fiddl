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
    //toggleSustain.addListener (this);
    
    addAndMakeVisible (toggleImpulse);
    toggleImpulse.setButtonText ("Impulse");
    //toggleImpulse.addListener (this);

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

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    //g.setColour (Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void ToggleSpaceComponent::resized()
{
    toggleSustain.setBounds(getWidth()-25, 10-getHeight()/2, 50,50);
    toggleImpulse.setBounds(getWidth()-25, 30-getHeight()/2, 50,50);
}
