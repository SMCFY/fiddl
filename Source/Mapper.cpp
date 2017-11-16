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
int Mapper::toggleSpaceVal;

void Mapper::routeParameters(int map) // all the mapping are defined here, and the values updated for AudioParameters
{
    mapping.clear();
    
    switch (toggleSpaceVal) {
        case 1:
            if (map == 1)
            {
                mapFromTo("y position","gain"); // call a method like this to do a mapping from a gesture to audio parameter
            }
            if (map == 2)
            {
                mapFromTo("y position","pitch");
            }
            break;
        case 2:
            if (map == 1)
            {
                mapFromTo("x position","lowpass"); // call a method like this to do a mapping from a gesture to audio parameter
            }
            if (map == 2)
            {
                mapFromTo("x position","pitch");
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
}

void Mapper::mapToTempo(float val)
{
    *AudioProcessorBundler::tempo = -95.0f + val*190.0f;
}

void Mapper::mapToLowPass(float val)
{
    *AudioProcessorBundler::lowPassFilterFreqParam = 20.0f + val*19980.0f;
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
                mapToGain(Gesture::getFingerPosition(0).x);
            }
            if (audioParameter == "pitch")     // ... to pitch value
            {
                mapToPitch(Gesture::getFingerPosition(0).x);
            }
            if (audioParameter == "tempo")     // ... to tempo value
            {
                mapToTempo(Gesture::getFingerPosition(0).x);
            }
            if (audioParameter == "lowpass")     // ... to tempo value
            {
                mapToLowPass(Gesture::getFingerPosition(0).x);
            }
        }
        if (gestureParameter == "y position") // mapping is being done from y position value ...
        {
            if (audioParameter == "gain")
            {
                mapToGain(Gesture::getFingerPosition(0).y); // ... to gain value
            }
            if (audioParameter == "pitch")     // ... to pitch value
            {
                mapToPitch(Gesture::getFingerPosition(0).y);
            }
            if (audioParameter == "tempo")     // ... to tempo value
            {
                mapToTempo(Gesture::getFingerPosition(0).y);
            }
            if (audioParameter == "lowpass")     // ... to tempo value
            {
                mapToLowPass(Gesture::getFingerPosition(0).y);
            }
        }
    }
}

void Mapper::setToggleSpace(int t)
{
    toggleSpaceVal = t;
}

std::vector< std::pair <std::string,std::string> > Mapper::mapping;
