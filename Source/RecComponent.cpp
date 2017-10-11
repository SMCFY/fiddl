/*
  ==============================================================================

    RecComponent.cpp
    Created: 11 Oct 2017 1:06:39pm
    Author:  Jonas Holfelt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RecComponent.h"

//==============================================================================
RecComponent::RecComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible (recordButton);
    recordButton.setButtonText ("Hold to record");
    recordButton.addListener(this);
    isRecording = false;

}

RecComponent::~RecComponent()
{
}

void RecComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

}

void RecComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    recordButton.setBounds(50,10,getWidth()/2,getHeight()-100);

}

void RecComponent::buttonClicked (Button* button)
    {
        if (button == &recordButton)
        {
            if (recorder->isRecording())
                stopRecording();
            else
                startRecording();
        }
    }