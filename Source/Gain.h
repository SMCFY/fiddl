/*
  ==============================================================================

    Gain.h
    Created: 23 Oct 2017 12:36:57pm
    Authors: Michael Castanieto
             Gergely Csapo
             Jonas Holfelt
    
    Description:  Handles the output gain.

  ==============================================================================
*/

#pragma once

#include "DSP.h"

class Gain : public DSP
{
public:
	Gain(AudioParameterFloat* gain);
	~Gain();

	void process(AudioBuffer<float> buffer) override;

private:
	AudioParameterFloat* gain;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain);
};
