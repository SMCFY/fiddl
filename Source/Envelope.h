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
		float envelope(int attackTime, float peak, int releaseTime); // AR envelope
		float envelope(int attackTime, float peak, int decayTime, float sustainLevel, int releaseTime); // ADSR envelope

		bool* isTriggered; // takes the address of isPlaying

	private:
		float amplitude;
		bool trig;
		int samplingRate;
		float aMin;
		bool noteOn;

		// phase conditions
		bool attack;
    	bool decay;
    	bool release;
		
		// amplitude increment/decrement
    	float attDelta;
    	float decDelta; 
    	float relDelta;
	
};
