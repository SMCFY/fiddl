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
#include "Envelope.h"
#include "RecComponent.h"

//==============================================================================
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

    static void stopPlaying(); // stop audio playback
    static void startPlaying(); // start audio playback
    
    static bool isPlaying; // true if recorded audio is playing
    static bool initRead; // resets readIndex in MCC
    
    //for ToggleSpace buttons
    int getToggleSpaceID();
    void buttonClicked (Button* button) override;
    int getToggleSpace();
    void setToggleSpace(int id);

    bool getLoopState();
    bool getDiscreteState();
    
    //velocity rolloff
    float velocityRolloff;
    void startRolloff();
    
    // get pointer to recComp
    void setRecComp(RecComponent *recComp);

    typedef struct Ripple
    {
        Point<float> pos;
        int index;
        float alpha;
        float circleSize;
        float line;
        int acc;
        Ripple(Point<float> p, int i)
        {
            this->pos = p;
            this->index = i;
            this->alpha = 1.0f;
            this->circleSize = 1.0f;
            this->line = 3.0f;
            this->acc = 15;
        }
    }Ripple;

    Envelope ar;
    Envelope adsr;

private:
    //Detect tap and/or direction of swipe
    float tapDetectCoords [2][2];
    float coordinates [20][2];
    int coordIndex = 0;
    bool swipeEnd = false;
    
    //Togglespace buttons
    ImageButton toggleSustain;
    ImageButton toggleImpulse;
    ImageButton toggleDiscrete;
    ImageButton toggleLoop;
    int toggleSpaceID = 1;
    
    //GUI
    float tipSize = 50;
    float tipThickness = 3;
    float pinchSize;
    float pinchThickness = 1;
    float lineThickness = 2;
    int rectListSize = 8;
    int rectNum = rectListSize;
    void drawPitchBar(Graphics& g);
    void timerCallback() override;
    void addRipple();
    void rmRipple(int i);
    void drawRipples(Graphics& g);
    void drawPinchIndicator(Graphics &g);
    void drawImpulseBackdrop(Graphics& g);
    OwnedArray<Ripple> ripples;
    RectangleList<float> rectList;

    bool pathEnabled;

    //Background Discrete Pitch GUI
    bool discretePitchToggled = false;
    bool loopToggled = false;
    void drawPitchBackDrop(Graphics& g);
    RectangleList<float> rectListBackDrop;
    
    //Background image
    Image sustainBackgroundImage;
    Image sustainButtonIconImage;
    Image impulseBackgroundImage;
    Image impulseButtonIconImage;
    Image discreteButtonIconImage;
    Image loopButtonIconImage;
    
    // pointer to recorder component
    RecComponent *recComp;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayComponent)
};
