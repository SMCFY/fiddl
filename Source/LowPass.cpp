/*
  ==============================================================================

    LowPass.cpp
    Created: 14 Nov 2017 9:41:43pm
    Author:  geri

  ==============================================================================
*/

#include "LowPass.h"
#include "AudioProcessorBundler.h"

LowPass::LowPass(AudioParameterFloat* cutoff)
{
	this->cutoff = cutoff;
    //lp.setCoefficients(IIRCoefficients::makeLowPass(44100, cutoff->get(), 0.1));
    dsp::ProcessSpec spec { 44100, static_cast<uint32> (512), 2 };
    
    lowPassFilter.state  = dsp::IIR::Coefficients<float>::makeLowPass  (44100, cutoff->get());
    lowPassFilter.prepare (spec);
}

LowPass::~LowPass()
{

}

void LowPass::process(AudioBuffer<float> buffer) 
{
    //lp.setCoefficients(IIRCoefficients::makeLowPass(samprate, cutoff->get(), Q));
    
	/*float** filterBuffer = buffer.getArrayOfWritePointers();
    
    for (int ch = 0; ch < buffer.getNumChannels(); ch++)
    {
        lp.processSamples(filterBuffer[ch], buffer.getNumSamples());
        
    }*/
    ScopedNoDenormals noDenormals;
    
    lowPassFilter.state  = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass  (44100, cutoff->get());
    dsp::AudioBlock<float> block (buffer);
    lowPassFilter.process (dsp::ProcessContextReplacing<float> (block));
    auto firstChan = block.getSingleChannelBlock (0);
	for (size_t chan = 1; chan < block.getNumChannels(); ++chan)
            block.getSingleChannelBlock (chan).copy (firstChan);
}

void LowPass::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    
    //lowPassFilter.reset();
}

