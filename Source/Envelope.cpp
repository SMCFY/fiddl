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
    this->sustain = 0;
}

Envelope::Envelope(int sr)
: aMin(0.001f)
{
	this->samplingRate = sr;
	this->amplitude = 0;
	this->sustain = 0;
}

Envelope::~Envelope()
{

}

void Envelope::trigger(bool trig)
{
	this->trig = trig;

// note off
	if(!trig)
		sustain = 0;
}

float Envelope::envelope(int attackTime, float peak, int releaseTime, bool& isTriggered) // AR
{

    if(trig) // init on trigger/re-trigger
	{
		amplitude = 0;
 
 		attack = 1;
    	release = 0;
	
    	attDelta = peak / std::round(samplingRate * (attackTime/1000));
    	relDelta = pow(aMin, peak / std::round(samplingRate * (releaseTime/1000)));

    	trig = 0;
   	}

    
	if(attack) // attack phase
	{
        amplitude += attDelta;
        if(amplitude >= peak)
        {
            amplitude = peak;
            attack = 0;
            release = 1;
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

float Envelope::envelope(int attackTime, float peak, int decayTime, float sustainLevel, int releaseTime, bool& isTriggered) // ADSR
{
	if(trig && !sustain) // init on trigger/re-trigger
	{
		amplitude = 0;
	
		attack = 1;
		decay = 0;
		sustain = 1;
    	release = 0;
	
    	attDelta = peak / std::round(samplingRate * (attackTime/1000));
    	decDelta = pow(aMin, peak / std::round(samplingRate * (decayTime/1000)));
    	relDelta = pow(aMin, sustainLevel / std::round(samplingRate * (releaseTime/1000)));

    	// trig = 0; std::cout << "trigoff";
   	}

    
	if(attack) // attack phase
	{
		amplitude += attDelta;
		if (amplitude >= peak)
		{
            amplitude = peak;
			attack = 0;
			decay = 1;
		}
		return amplitude;
	}
	else if(decay && amplitude > sustainLevel) // decay phase
	{
		amplitude *= decDelta;
		return amplitude;
	}
	else if(sustain && amplitude <= sustainLevel) // sustain phase
	{
		amplitude = sustainLevel;
		decay = 0;
		release = 1;
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
