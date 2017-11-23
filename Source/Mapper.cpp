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
                mapFromTo(X_POSITION, PITCH);
                mapFromTo(Y_POSITION, BANDPASS);
            }
            if (numFingers >= 2)
            {
                mapFromTo(Y_POSITION, PITCH);
                mapFromTo(X_POSITION, LOWPASS);
            }
            if (isInPitchBar)
            {
                mapFromTo(X_POSITION, DISCRETE_PITCH);
                //mapFromTo(Y_POSITION, LOWPASS);
            }
            break;
        case 2: // impulse
            if (numFingers == 1)
            {
                mapFromTo(ABS_DIST, PITCH);
                mapFromTo(ABS_DIST, HIGHPASS);
            }
            if (numFingers >= 2)
            {
                mapFromTo(Y_POSITION, PITCH);
                mapFromTo(X_POSITION, HIGHPASS);
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

void Mapper::mapFromTo(const GestureParameter gestureParameter, const AudioParameter audioParameter)
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
    GestureParameter gestureParameter;
    AudioParameter audioParameter;
    //iterate through the mapping string pairs to map a "gestureParameter" to "audioParameter"
    for (std::vector<std::pair<GestureParameter, AudioParameter>>::iterator it = mapping.begin() ; it != mapping.end(); ++it)
    {
        gestureParameter = it->first;
        audioParameter = it->second;

        switch (gestureParameter) {
            case X_POSITION:
                switch (audioParameter) {
                    case GAIN:
                        mapToGain(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                    case PITCH:
                        mapToPitch(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                    case DISCRETE_PITCH:
                        mapToDiscretePitch(Gesture::getDiscretePitch());
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                    case LOWPASS:
                        mapToLowPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                    case HIGHPASS:
                        mapToHighPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                    case BANDPASS:
                        mapToBandPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                }
                break;
            case Y_POSITION:
                switch (audioParameter) {
                    case GAIN:
                        mapToGain(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        break;
                    case PITCH:
                        mapToPitch(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        break;
                    case DISCRETE_PITCH:
                        /* Y_POSITION currently not mapped to DISCRETE_PITCH */
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        break;
                    case LOWPASS:
                        mapToLowPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        break;
                    case HIGHPASS:
                        mapToHighPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        break;
                    case BANDPASS:
                        mapToBandPass(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        break;
                }
                break;
            case ABS_DIST:
                switch (audioParameter) {
                    case GAIN:
                        mapToGain(Gesture::getAbsDistFromOrigin());
                        break;
                    case PITCH:
                        mapToPitch(Gesture::getAbsDistFromOrigin());
                        break;
                    case DISCRETE_PITCH:
                        /* ABS_DIST currently not mapped to DISCRETE_PITCH */
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getAbsDistFromOrigin());
                        break;
                    case LOWPASS:
                        mapToLowPass(Gesture::getAbsDistFromOrigin());
                        break;
                    case HIGHPASS:
                        mapToHighPass(Gesture::getAbsDistFromOrigin());
                        break;
                    case BANDPASS:
                        mapToHighPass(Gesture::getAbsDistFromOrigin());
                        break;
                }
                break;
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

std::vector< std::pair <GestureParameter,AudioParameter> > Mapper::mapping;
