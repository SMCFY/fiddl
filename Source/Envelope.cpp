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
{
    this->samplingRate = 44100;
}

Envelope::Envelope(int sr)
{
	this->samplingRate = sr;
	this->amplitude = 0;
}

Envelope::~Envelope()
{

}

void Envelope::trigger(bool trig)
{
	this->trig = trig;
}

float Envelope::envelope(int attackTime, float peak, int releaseTime, bool& isTriggered)
{
	if(trig) // init on trigger/re-trigger
	{
		amplitude = 0;
	
		attack = 1;
    	release = 0;
	
    	attDelta = peak / std::round(samplingRate * (attackTime/1000));
    	relDelta = peak / std::round(samplingRate * (releaseTime/1000));
   	}

    
	if(attack && amplitude < peak) // attack phase
	{
		amplitude += attDelta;
		return amplitude;
	}
	else if(attack && amplitude >= peak) // cap amplitude to peak
	{
		amplitude = peak;
		attack = 0;
		release = 1;
		return amplitude;
	}
	else if(release && amplitude >= 0) // release phase
	{
		amplitude -= relDelta;
		return amplitude;
	}
	else if(release && amplitude < 0)
	{	
		amplitude = 0;
		release = 0;
		isTriggered = 0; // set isPlaying false and resets readIndex
		return 0.0f;
	}
}

float Envelope::envelope(int attackTime, float peak, int decayTime, float sustainLevel, int releaseTime)
{
	return 0.0;
}

float Envelope::getValue()
{
	return amplitude;
}
