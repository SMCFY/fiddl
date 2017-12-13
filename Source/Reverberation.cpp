


#include "Reverberation.h"
#include "AudioProcessorBundler.h"

Reverberation::Reverberation(int sampleRate)
{
    reverb = new Reverb();
    reverb->setSampleRate(sampleRate);
}

Reverberation::~Reverberation()
{

}

void Reverberation::process(AudioBuffer<float> buffer)
{
    float** buff = buffer.getArrayOfWritePointers();
    reverb->processMono(*buff, buffer.getNumSamples());
    std::cout << "reverb is processing...";
}

void Reverberation::updateParameters(float roomSize, float damping, float wetLevel, float dryLevel, float width, float freezeMode)
{
    params.roomSize = roomSize;
    params.damping = damping;
    params.wetLevel = wetLevel;
    params.dryLevel = dryLevel;
    params.width = width;
    params.freezeMode = freezeMode;
    
    reverb->setParameters(params);
}
