/*
  ==============================================================================

    Mapper.cpp
    Created: 23 Oct 2017 11:56:31pm
    Author:  geri

  ==============================================================================
*/

#include "Mapper.h"

void Mapper::routeParameters() // all the mapping are defined here, and the values updated for AudioParameters
{
	mapToGain(1.0f);
}

void Mapper::mapToGain(float val)
{
	gainLevel->setValue(val);
}

AudioParameterFloat *Mapper::gainLevel = new AudioParameterFloat("gainLevel", "Gain", 0.0f, 0.5f, 1.0f);