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
	Filter(AudioParameterFloat* cutoff, AudioParameterFloat* q, const String filterType);
	~Filter();

	void process(AudioBuffer<float> buffer) override;

private:
	AudioParameterFloat* cutoff;
    AudioParameterFloat* q;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter, highPassFilter, bandPassFilter;
    bool isHighPass, isLowPass, isBandPass;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Filter);
};
