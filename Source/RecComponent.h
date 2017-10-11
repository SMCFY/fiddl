/*
  ==============================================================================

    RecComponent.h
    Created: 11 Oct 2017 1:06:39pm
    Author:  Jonas Holfelt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class RecComponent    : public Component,
                        public Button::Listener
{
public:
    RecComponent();
    ~RecComponent();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;

    void startRecording();
    void stopRecording();

    bool isRecording;
private:
    TextButton recordButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecComponent)
};
