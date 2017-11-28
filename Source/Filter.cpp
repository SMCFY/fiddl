/*
  ==============================================================================

    Filter.cpp
    Created: 14 Nov 2017 9:41:43pm
    Author:  geri

  ==============================================================================
*/

#include "Filter.h"
#include "AudioProcessorBundler.h"

Filter::Filter(AudioParameterFloat* cutoff, AudioParameterFloat* q, const String filterType)
: isHighPass(false), isLowPass(false), isBandPass(false)
{
    if (filterType == "lowpass")
    {
        isLowPass = true;
    }
    else if (filterType == "highpass")
    {
        isHighPass = true;
    }
    else if (filterType == "bandpass")
    {
        isBandPass = true;
    }
    
	this->cutoff = cutoff;
    this->q = q;
    dsp::ProcessSpec spec { 44100, static_cast<uint32> (512), 2 };
    
    *lowPassFilter.state  = *dsp::IIR::Coefficients<float>::makeLowPass  (44100, cutoff->get(), q->get());
    lowPassFilter.prepare (spec);
    *highPassFilter.state  = *dsp::IIR::Coefficients<float>::makeHighPass  (44100, cutoff->get(), q->get());
    highPassFilter.prepare (spec);
    *bandPassFilter.state  = *dsp::IIR::Coefficients<float>::makeBandPass  (44100, cutoff->get(), q->get());
    bandPassFilter.prepare (spec);
}

Filter::~Filter()
{

}

void Filter::process(AudioBuffer<float> buffer)
{
    ScopedNoDenormals noDenormals;
    
    dsp::AudioBlock<float> block (buffer);
    if (isLowPass)
    {
        *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass (44100, cutoff->get(), q->get());
        lowPassFilter.process (dsp::ProcessContextReplacing<float> (block));
    }
    else if (isHighPass)
    {
        *highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass  (44100, cutoff->get(), q->get());
        highPassFilter.process (dsp::ProcessContextReplacing<float> (block));
    }
    else if (isBandPass)
    {
        *bandPassFilter.state = *dsp::IIR::Coefficients<float>::makeBandPass  (44100, cutoff->get(), q->get());
        bandPassFilter.process (dsp::ProcessContextReplacing<float> (block));
    }
    
    
    auto firstChan = block.getSingleChannelBlock (0);
	for (size_t chan = 0; chan < block.getNumChannels(); ++chan)
            block.getSingleChannelBlock (chan).copy (firstChan);
}
