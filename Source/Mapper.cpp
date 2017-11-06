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

void Mapper::routeParameters() // all the mapping are defined here, and the values updated for AudioParameters
{
	
	mapFromTo("y position","gain"); // call a method like this to do a mapping from a gesture to audio parameter
		
}

void Mapper::mapToGain(float val)
{
    *AudioProcessorBundler::gainLevel = Mapper::lin2log(1,0.01,val);
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
                mapToGain(Gesture::getFingerPosition(0)->xPos);
            }
        }
        if (gestureParameter == "y position") // mapping is being done from y position value ...
        {
            if (audioParameter == "gain")
            {
                mapToGain(Gesture::getFingerPosition(0)->yPos); // ... to gain value
            }
        }
    }
}

std::vector< std::pair <std::string,std::string> > Mapper::mapping;
