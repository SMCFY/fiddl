/*
  ==============================================================================

    Filter.h
    Created: 14 Nov 2017 9:41:43pm
    Author:  geri

  ==============================================================================
*/

#pragma once
#include "DSP.h"

class Filter : public DSP
{
public:
	Filter(AudioParameterFloat* cutoff, const String filterType);
	~Filter();

	void process(AudioBuffer<float> buffer) override;

private:
	AudioParameterFloat* cutoff;
	IIRFilter lp;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter, highPassFilter;
    bool isHighPass, isLowPass;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Filter);
};
