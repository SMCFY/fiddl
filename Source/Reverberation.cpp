


#include "Reverberation.h"
#include "AudioProcessorBundler.h"

Reverberation::Reverberation(AudioParameterFloat* roomSize, AudioParameterFloat* damping, AudioParameterFloat* wetLevel, AudioParameterFloat* dryLevel, AudioParameterFloat* width, AudioParameterFloat* freezeMode, double sampleRate)
{
    reverb = new Reverb();
    
    reverb->setSampleRate(sampleRate);
    
    params.roomSize = *roomSize;
    params.damping = *damping;
    params.wetLevel = *wetLevel;
    params.dryLevel = *dryLevel;
    params.width = *width;
    params.freezeMode = *freezeMode;
    
    this->roomSize = roomSize;
    this->damping = damping;
    this->wetLevel = wetLevel;
    this->dryLevel = dryLevel;
    this->width = width;
    this->freezeMode = freezeMode;
    
    reverb->setParameters(params);
}

Reverberation::~Reverberation()
{

}

void Reverberation::process(AudioBuffer<float> buffer)
{
    updateParameters();
    reverb->setParameters(params);
    reverb->processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
}

void Reverberation::updateParameters()
{
    params.roomSize = roomSize->get();
    params.damping = damping->get();
    params.wetLevel = wetLevel->get();
    params.dryLevel = dryLevel->get();
    params.width = width->get();
    params.freezeMode = freezeMode->get();
    
    reverb->setParameters(params);
}
