


#pragma once
#include "DSP.h"

class Reverberation : public DSP
{
public:
    Reverberation(int sampleRate);
	~Reverberation();

	void process(AudioBuffer<float> buffer) override;
    void updateParameters(float roomSize, float damping, float wetLevel, float dryLevel, float width, float freezeMode);

private:
    
    Reverb *reverb;
    Reverb::Parameters params;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Reverberation);
};
