/*
  ==============================================================================

    Mapper.cpp
    Created: 23 Oct 2017 11:56:31pm
    Author:  geri

  ==============================================================================
*/

#include "Mapper.h"
#include "Gesture.h"

void Mapper::routeParameters() // all the mapping are defined here, and the values updated for AudioParameters
{
	//mapToGain(1.0f);
	mapFromTo("x position","gain"); // call a method like this to do a mapping from a gesture to audio parameter
}

void Mapper::mapToGain(float val)
{
	*gainLevel = val;
}

void Mapper::mapFromTo(const std::string gestureParameter, const std::string audioParameter)
{
	mapping.push_back(std::make_pair(gestureParameter,audioParameter));
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
                mapToGain(Gesture::getXPos());
            }
        }
        if (gestureParameter == "y position") // mapping is being done from y position value ...
        {
            if (audioParameter == "gain")
            {
                mapToGain(Gesture::getYPos()); // ... to gain value
            }
        }
    }
}

AudioParameterFloat *Mapper::gainLevel = new AudioParameterFloat("gainLevel", "Gain", 0.0f, 0.5f, 1.0f);
std::vector< std::pair <std::string,std::string> > Mapper::mapping;
