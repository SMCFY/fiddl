/*
  ==============================================================================

    Envelope.h
    Created: 15 Nov 2017 11:49:39pm
    Author:  geri

  ==============================================================================
*/

#pragma once

class Envelope
{
	public:
        Envelope();
		Envelope(int sr);
		~Envelope();

		void trigger(bool trig);
		float envelope(int attackTime, float peak, int releaseTime, bool& isTriggered); // AR envelope
		float envelope(int attackTime, float peak, int decayTime, float sustainLevel, int releaseTime, bool& isTriggered); // ADSR envelope

	private:
		float amplitude;
		bool trig;
		int samplingRate;

		bool attack;
    	bool decay;
    	bool sustain;
    	bool release;
	
    	float attDelta;
    	float decDelta; 
    	float relDelta;
	
};
