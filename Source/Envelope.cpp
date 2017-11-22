/*
  ==============================================================================

    Envelope.cpp
    Created: 15 Nov 2017 11:49:39pm
    Author:  geri

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"
Envelope::Envelope()
: aMin(0.001f)
{
    this->samplingRate = 44100;
    this->amplitude = 0;
    this->noteOn = 0;
}

Envelope::Envelope(int sr, Envelope::env type)
: aMin(0.001f)
{
	this->samplingRate = sr;
	this->amplitude = 0;
	this->noteOn = 0;

	//if(type == "ar")
	//	this->envelopeType = ar;
	//else if(type == "adsr")
	//	this->envelopeType = adsr;
	this->envelopeType = type;
	
}

Envelope::~Envelope()
{

}

void Envelope::trigger(bool trig)
{
	this->trig = trig;
	
	if (trig) // note on
		amplitude = 0;

	if(!trig) // note off
		noteOn = 0;
}

float Envelope::envelope(int attackTime, float peak, int releaseTime) // AR
{

    if(trig && !noteOn) // init on trigger/re-trigger
	{
 		attack = 1;
    	release = 1;

    	trig = 0;
	
    	attDelta = peak / std::round(samplingRate * (attackTime/1000));
    	relDelta = pow(aMin, peak / std::round(samplingRate * (releaseTime/1000)));
   	}

    
	if(attack) // attack phase
	{
        amplitude += attDelta;
        if(amplitude >= peak)
        {
            amplitude = peak;
            attack = 0;
            return amplitude;
        }
		return amplitude;
	}
	else if(release) // release phase
	{
		if(amplitude >= aMin)
		{
			amplitude *= relDelta;
			return amplitude;
		}
		else
		{
			amplitude = 0;
			release = 0;
			isTriggered = 0; // set isPlaying false and resets readIndex
			return 0.0f;
		}
	}
	
    return 0.0f;
}

float Envelope::envelope(int attackTime, float peak, int decayTime, float sustainLevel, int releaseTime) // ADSR
{
	if(trig && !noteOn) // init on trigger/re-trigger
	{
		attack = 1;
		decay = 1;
    	release = 1;
    	noteOn = 1;
	
    	attDelta = peak / std::round(samplingRate * (attackTime/1000));
    	decDelta = pow(aMin, peak / std::round(samplingRate * (decayTime/1000)));
    	relDelta = pow(aMin, sustainLevel / std::round(samplingRate * (releaseTime/1000)));
   	}

    
	if(attack) // attack phase
	{
		amplitude += attDelta;
		if (amplitude >= peak)
		{
            amplitude = peak;
			attack = 0;
		}
		return amplitude;
	}
	else if(decay && amplitude > sustainLevel) // decay phase
	{
		amplitude *= decDelta;
		if (attack <= sustainLevel)
		{
			amplitude = sustainLevel;
			decay = 0;
		}
		return amplitude;
	}
	else if(noteOn) // sustain phase
	{
		amplitude = sustainLevel;
		return amplitude;
	}
	else if(release) // release phase
	{
		amplitude *= relDelta;
		if(amplitude <= aMin)
		{
			amplitude = 0;
			release = 0;
			isTriggered = 0; // set isPlaying false and resets readIndex			
		}
		return amplitude;
	}

    return 0.0f;
}

void Envelope::process(AudioBuffer<float> buffer)
{
	float **outputFrame = buffer.getArrayOfWritePointers();

	for (int samp = 0; samp < buffer.getNumSamples(); ++samp)
	{
	    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
	    {
	    	switch (envelopeType)
	    	{
            case ar:
	    	outputFrame[ch][samp] *= envelope(50, 0.95, 1000); // APR
	        break;
                    
            case adsr:
	        outputFrame[ch][samp] *= envelope(1000, 0.95, 500, 0.8, 2000); // APDSR
	        break;
            };
	    }
	}
}
