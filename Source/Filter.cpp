/*
  ==============================================================================

    Filter.cpp
    Created: 14 Nov 2017 9:41:43pm
    Author:  geri

  ==============================================================================
*/

#include "Filter.h"
#include "AudioProcessorBundler.h"

Filter::Filter(AudioParameterFloat* cutoff, const String filterType)
: isHighPass(false), isLowPass(false)
{
    if (filterType == "lowpass")
    {
        isLowPass = true;
    }
    else if (filterType == "highpass")
    {
        isHighPass = true;
    }
    
	this->cutoff = cutoff;
    dsp::ProcessSpec spec { 44100, static_cast<uint32> (512), 2 };
    
    *lowPassFilter.state  = *dsp::IIR::Coefficients<float>::makeLowPass  (44100, cutoff->get());
    lowPassFilter.prepare (spec);
    *highPassFilter.state  = *dsp::IIR::Coefficients<float>::makeHighPass  (44100, cutoff->get());
    highPassFilter.prepare (spec);
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
        *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass (44100, cutoff->get());
        lowPassFilter.process (dsp::ProcessContextReplacing<float> (block));
    }
    else if (isHighPass)
    {
        *highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass  (44100, cutoff->get());
        highPassFilter.process (dsp::ProcessContextReplacing<float> (block));
    }
    
    
    auto firstChan = block.getSingleChannelBlock (0);
	for (size_t chan = 0; chan < block.getNumChannels(); ++chan)
            block.getSingleChannelBlock (chan).copy (firstChan);
}
