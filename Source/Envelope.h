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
        typedef enum env{AR, ADSR} env;

        Envelope();
		Envelope(int sr, Envelope::env type);
		~Envelope();

		void trigger(bool trig);
		float envelope(int attackTime, float peak, int releaseTime); // AR envelope
		float envelope(int attackTime, float peak, int decayTime, float sustainLevel, int releaseTime); // ADSR envelope
		void process(AudioBuffer<float> buffer); // processses an audio buffer based on the envelope type

		void setReleaseTime(int time);

		bool* isTriggered; // takes the address of isPlaying
    
	private:
		float amplitude;
		bool trig;
		int samplingRate;
		float aMin;
		bool noteOn;

		int releaseTime;
		env envelopeType; 

		// phase conditions
		bool attack;
    	bool decay;
    	bool release;
		
		// amplitude increment/decrement
    	float attDelta;
    	float decDelta; 
    	float relDelta;
	
};
