/*
  ==============================================================================

    Envelope.cpp
    Created: 15 Nov 2017 11:49:39pm
    Author:  geri

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"
#include "Mapper.h"
#include "PlayComponent.h"

Envelope::Envelope()
: aMin(0.001f), trig(0)
{
    this->samplingRate = 44100;
    this->amplitude = 0;
    this->noteOn = 0;
    this->rampDown = 0;
}

Envelope::Envelope(Envelope::env type)
: aMin(0.001f), trig(0)
{
	this->samplingRate = 44100;
	this->amplitude = 0;
	this->noteOn = 0;
	this->rampDown = 0;

	this->envelopeType = type;
    this->releaseTime = &Mapper::releaseT;
}

Envelope::~Envelope()
{

}

void Envelope::trigger(bool trig)
{
	this->trig = trig;
 	
 	//if(trig) // note on
 	//{
	//	amplitude = 0;
	//	PlayComponent::startPlaying();
 	//}

	if(!trig) // note off
		noteOn = 0;
}

float Envelope::envelope(int attackTime, float peak, int releaseTime) // AR
{
    relDelta = pow(aMin, peak / std::round(samplingRate * (releaseTime/1000)));

    if(trig && !noteOn) // init on trigger/re-trigger
	{
 		attack = 1;
    	release = 1;

    	trig = 0;
	
    	if(amplitude >= aMin) // ramp down envelope on re-trigger
    		rampDown = 1;
    	else
    		PlayComponent::startPlaying();

    	attDelta = peak / std::round(samplingRate * (attackTime/1000));
   	}

    if (rampDown)
    {
    	amplitude *= 0.999;
    	if (amplitude <= aMin)
    	{
    		amplitude = 0;
    		rampDown = 0;
    		PlayComponent::startPlaying(); // reset read index and initialize playback
        }
    	return amplitude;
    }
	else if(attack) // attack phase
	{
        amplitude += attDelta;
        if(amplitude >= peak)
        {
            amplitude = peak;
            attack = 0;
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
			PlayComponent::stopPlaying(); // set isPlaying false and resets readIndex
			return 0.0f;
		}
	}
	
    return 0.0f;
}

float Envelope::envelope(int attackTime, float peak, int decayTime, float sustainLevel, int releaseTime) // ADSR
{
    relDelta = pow(aMin, sustainLevel / std::round(samplingRate * (releaseTime/1000)));

	if(trig && !noteOn) // init on trigger/re-trigger
	{
		attack = 1;
		decay = 1;
		noteOn = 1; // sustain
    	release = 1;

    	PlayComponent::startPlaying(); std::cout<< "REALLLY";
	    
    	attDelta = peak / std::round(samplingRate * (attackTime/1000));
    	decDelta = pow(aMin, peak / std::round(samplingRate * (decayTime/1000)));
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
			PlayComponent::stopPlaying(); // set isPlaying false and resets readIndex			
		}
		return amplitude;
	}

    return 0.0f;
}

void Envelope::generateRamp(float start, float end, int lengthInSamples, String type)
{
    if(type == "exp")
    {
        float k = pow(end, start / lengthInSamples);
        float val = start;

        for (int i = 0; i < lengthInSamples; i++)
        {
            Envelope::ramp[i] = val;
            val *= k;
        }
    }
    else if(type == "lin")
    {
        float k = (end-start) / lengthInSamples;
        float val = start;

        for (int i = 0; i < lengthInSamples; i++)
        {
            Envelope::ramp[i] = val;
            val += k;
        }
    }
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
            case AR:
	    	outputFrame[ch][samp] *= envelope(50, 0.90, *releaseTime); // APR
	        break;
                    
            case ADSR:
	        outputFrame[ch][samp] *= envelope(1000, 0.95, 500, 0.8, *releaseTime); // APDSR
	        break;
            };
	    }
	}
	
}

void Envelope::setReleaseTime(int &time)
{
	this->releaseTime = &time;
}

void Envelope::setSamplingRate(int sr)
{
	this->samplingRate = sr;
}

float Envelope::getAmplitude()
{
    return amplitude;
}

float Envelope::ramp[96000];
