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
		void setReleaseTime(int &time);
		void setSamplingRate(int sr);
    
        float getAmplitude();
    
        static void generateRamp(float start, float end, int lengthInSamples, String type);
    	static float ramp[96000];

	private:
		float amplitude;
		bool trig;
		int samplingRate;
		float aMin;
		bool noteOn;

		int* releaseTime;
		env envelopeType; 

		// phase conditions
		bool attack;
    	bool decay;
    	bool release;
    	bool rampDown;
		
		// amplitude increment/decrement
    	float attDelta;
    	float decDelta; 
    	float relDelta;
	
};
