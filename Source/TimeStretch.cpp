/*
  ==============================================================================

    TimeStretch.cpp
    Created: 20 Oct 2017 3:13:32pm
    Author:  Michael Castanieto

  ==============================================================================
*/

#include "TimeStretch.h"

TimeStretch::TimeStretch()
{
    // initialise the DSP state array, something like this:
    /*
    state = new float*[numChannels];
    for (int ch = 0; ch < numChannels; ch++)
    {
        state[ch] = new float[bufferLengthInSamples];
    }
    */
    
    // add audio parameters like this:
    addParameter (parameter1 = new AudioParameterFloat ("param1", // parameter ID
                                                        "Parameter 1", // parameter name
                                                        0.0f,   // mininum value
                                                        1.0f,   // maximum value
                                                        0.5f)); // default value
    addParameter (parameter2 = new AudioParameterFloat ("param2", 
                                                        "Parameter 2", 
                                                        0.0f,   
                                                        1.0f,   
                                                        0.5f)); 
}

TimeStretch::~TimeStretch()
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

void TimeStretch::process(AudioBuffer<float> buffer)
{
    /* here is where all the timestretching magic happens ;) */
}