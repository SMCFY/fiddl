/*
  ==============================================================================

    Gain.cpp
    Created: 23 Oct 2017 12:36:57pm
    Authors: Michael Castanieto
             Gergely Csapo
             Jonas Holfelt

  ==============================================================================
*/

#include "Gain.h"

Gain::Gain(AudioParameterFloat *gain)
{
    // initialise the DSP state array, something like this:
    /*
    state = new float*[numChannels];
    for (int ch = 0; ch < numChannels; ch++)
    {
        state[ch] = new float[bufferLengthInSamples];
    }
    */
    
    this->gain = gain;
}

Gain::~Gain()
{
    // delete the state array, something like this:
    /*
    for (int i = 0; i < numChannels; i++)
    {
        delete state[i];
    }
    delete state;
    */
}

void Gain::process(AudioBuffer<float> buffer)
{
    /* set the gain level here */
}
