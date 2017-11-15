/*
  ==============================================================================

    LowPass.cpp
    Created: 14 Nov 2017 9:41:43pm
    Author:  geri

  ==============================================================================
*/

#include "LowPass.h"

LowPass::LowPass(AudioParameterFloat* cutoff)
{
	this->lpCutOff = cutoff;
    lp.setCoefficients(IIRCoefficients::makeLowPass(44100, cutoff->get(), 0.1));
}

LowPass::~LowPass()
{

}

void LowPass::process(AudioBuffer<float> buffer) 
{
    //lp.setCoefficients(IIRCoefficients::makeLowPass(samprate, cutoff->get(), Q));
    
	float** filterBuffer = buffer.getArrayOfWritePointers();
    
    for (int ch = 0; ch < buffer.getNumChannels(); ch++)
    {
        lp.processSamples(filterBuffer[ch], buffer.getNumSamples());
        
    }
	
}
