/*
  ==============================================================================

    Envelope.cpp
    Created: 15 Nov 2017 11:49:39pm
    Author:  geri

  ==============================================================================
*/

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

	if(trig)
	this->amplitude = 0; // init amplitude on trigger
}

float Envelope::envelope(int attackTime, float peak, int releaseTime)
{
	//if(trig && !attack && !release) // init on trigger
	//{
	bool attack = 1;
    bool release = 0;

    float attackInSamples = samplingRate / (attackTime/1000);
    float releaseInSamples = samplingRate / (releaseTime/1000);
    float attDelta = peak / attackInSamples;
    float relDelta = peak / releaseInSamples;
   	//}
    
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
	else if(release && amplitude >= 0)// release phase
	{
		amplitude -= relDelta;
		return amplitude;
	}
	else
	{	
		amplitude = 0;
		release = 0;
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
