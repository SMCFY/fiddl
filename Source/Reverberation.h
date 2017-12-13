


#pragma once
#include "DSP.h"

class Reverberation : public DSP
{
public:
    Reverberation(AudioParameterFloat* roomSize, AudioParameterFloat* damping, AudioParameterFloat* wetLevel, AudioParameterFloat* dryLevel, AudioParameterFloat* width, AudioParameterFloat* freezeMode, double sampleRate);
	~Reverberation();

	void process(AudioBuffer<float> buffer) override;
    void updateParameters();

private:
    
    Reverb *reverb;
    Reverb::Parameters params;
    AudioParameterFloat* roomSize;
    AudioParameterFloat* damping;
    AudioParameterFloat* wetLevel;
    AudioParameterFloat* dryLevel;
    AudioParameterFloat* width;
    AudioParameterFloat* freezeMode;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Reverberation);
};
