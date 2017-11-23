/*
  ==============================================================================

    Mapper.cpp
    Created: 23 Oct 2017 11:56:31pm
    Author:  geri

  ==============================================================================
*/

#include "Mapper.h"
#include "Gesture.h"
#include "AudioProcessorBundler.h"

float Mapper::inMax;
float Mapper::inMin;
int Mapper::toggleSpaceID;

void Mapper::routeParameters(int numFingers, bool isInPitchBar) // all the mapping are defined here, and the values updated for AudioParameters
{
    mapping.clear();
    
    switch (toggleSpaceID) {
        case 1: // sustain
            if (numFingers == 1)
            {
                mapFromTo("x position","pitch");
                mapFromTo("y position", "bandpass");
            }
            if (numFingers >= 2)
            {
                mapFromTo("y position","pitch");
                mapFromTo("x position", "lowpass");
            }
            if (isInPitchBar)
            {
                mapFromTo("x position","discrete pitch");
                //mapFromTo("y position", "lowpass");
            }
            break;
        case 2: // impulse
            if (numFingers == 1)
            {
                mapFromTo("abs dist","pitch");
                mapFromTo("abs dist","highpass");
            }
            if (numFingers >= 2)
            {
                mapFromTo("y position","pitch");
                mapFromTo("x position","highpass");
            }
        default:
            break;
    }
}

void Mapper::mapToGain(float val)
{
    *AudioProcessorBundler::gainLevel = Mapper::lin2log(1,0.01,val);
}

void Mapper::mapToPitch(float val)
{
    *AudioProcessorBundler::pitch = -12.0f + val*24.0f;
    AudioProcessorBundler::timeStretch->pitchUpdated = true;
}

void Mapper::mapToDiscretePitch(float val)
{
    *AudioProcessorBundler::pitch = val;
    AudioProcessorBundler::timeStretch->pitchUpdated = true;
}

void Mapper::mapToTempo(float val)
{
    *AudioProcessorBundler::tempo = -95.0f + val*190.0f;
}

void Mapper::mapToLowPass(float val)
{
    *AudioProcessorBundler::lowPassFilterFreqParam = 20.0f + val*3000.0f;
}

void Mapper::mapToHighPass(float val)
{
    *AudioProcessorBundler::highPassFilterFreqParam = 20.0f + val*3000.0f;
}

void Mapper::mapToBandPass(float val)
{
    *AudioProcessorBundler::bandPassFilterFreqParam = 20.0f + val*3000.0f;
}

void Mapper::mapFromTo(const std::string gestureParameter, const std::string audioParameter)
{
    mapping.push_back(std::make_pair(gestureParameter,audioParameter));
}

float Mapper::lin2log(float outMax, float outMin, float input)
{
    Mapper::inMax = 1;
    Mapper::inMin = 0.01;
    
    if (input > Mapper::inMax)
        input = Mapper::inMax;
    
    if (input < Mapper::inMin)
        input = Mapper::inMin;
    
    return outMax + (log(input) - log(Mapper::inMax)) / (log(Mapper::inMin) - log(Mapper::inMax)) * (outMin - outMax);
}

// this method is called in the PlayComponent class to update all the mapping values
void Mapper::updateParameters()
{
    std::string gestureParameter;
    std::string audioParameter;
    //iterate through the mapping string pairs to map a "gestureParameter" to "audioParameter"
    for (std::vector<std::pair<std::string, std::string>>::iterator it = mapping.begin() ; it != mapping.end(); ++it)
    {
        gestureParameter = it->first;
        audioParameter = it->second;
        if (gestureParameter == "x position") // mapping is being done from x position value ...
        {
            if (audioParameter == "gain")     // ... to gain value
            {
                mapToGain(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
            }
            if (audioParameter == "pitch")     // ... to pitch value
            {
                mapToPitch(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
            }
            if (audioParameter == "discrete pitch")     // ... to pitch value
            {
                mapToDiscretePitch(Gesture::getDiscretePitch());
            }
            if (audioParameter == "tempo")     // ... to tempo value
            {
                mapToTempo(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
            }
            if (audioParameter == "lowpass")     // ... to tempo value
            {
                mapToLowPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
            }
            if (audioParameter == "highpass")     // ... to tempo value
            {
                mapToHighPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
            }
            if (audioParameter == "bandpass")     // ... to tempo value
            {
                mapToBandPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
            }
        }
        if (gestureParameter == "y position") // mapping is being done from y position value ...
        {
            if (audioParameter == "gain")
            {
                mapToGain(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y); // ... to gain value
            }
            if (audioParameter == "pitch")     // ... to pitch value
            {
                mapToPitch(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
            }
            if (audioParameter == "tempo")     // ... to tempo value
            {
                mapToTempo(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
            }
            if (audioParameter == "lowpass")     // ... to tempo value
            {
                mapToLowPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
            }
            if (audioParameter == "highpass")     // ... to tempo value
            {
                mapToHighPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
            }
            if (audioParameter == "bandpass")     // ... to tempo value
            {
                mapToBandPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
            }
        }
        if (gestureParameter == "abs dist") // mapping is being done from the absolute distance from origin
        {
            if (audioParameter == "gain")
            {
                mapToGain(Gesture::getAbsDistFromOrigin()); // ... to gain value
            }
            if (audioParameter == "pitch")     // ... to pitch value
            {
                mapToPitch(Gesture::getAbsDistFromOrigin());
            }
            if (audioParameter == "tempo")     // ... to tempo value
            {
                mapToTempo(Gesture::getAbsDistFromOrigin());
            }
            if (audioParameter == "lowpass")     // ... to tempo value
            {
                mapToLowPass(Gesture::getAbsDistFromOrigin());
            }
            if (audioParameter == "highpass")     // ... to tempo value
            {
                mapToHighPass(Gesture::getAbsDistFromOrigin());
            }
            if (audioParameter == "bandpass")     // ... to tempo value
            {
                mapToBandPass(Gesture::getAbsDistFromOrigin());
            }
        }
    }
}

void Mapper::setToggleSpace(int id)
{
    toggleSpaceID = id;
}

int Mapper::getToggleSpaceID()
{
    return toggleSpaceID;
}

std::vector< std::pair <std::string,std::string> > Mapper::mapping;
