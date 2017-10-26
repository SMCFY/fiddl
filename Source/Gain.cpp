<<<<<<< HEAD
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
    this->addParameter(gain);
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

void Gain::process(AudioBuffer<float> *buffer)
{
    buffer.applyGain(this->gain);
}
=======
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
>>>>>>> 867e26ad08a567b9b337ad2e9b1984d43eac2b46
