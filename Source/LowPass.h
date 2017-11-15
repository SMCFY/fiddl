/*
  ==============================================================================

    LowPass.h
    Created: 14 Nov 2017 9:41:43pm
    Author:  geri

  ==============================================================================
*/

#pragma once
#include "DSP.h"

class LowPass : public DSP
{
public:
	LowPass(AudioParameterFloat* cutoff);
	~LowPass();

	void process(AudioBuffer<float> buffer) override;

private:
	AudioParameterFloat* lpCutOff;
	IIRFilter lp;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LowPass);
};