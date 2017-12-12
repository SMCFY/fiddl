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
#include "PlayComponent.h"

float Mapper::inMax;
float Mapper::inMin;
int Mapper::toggleSpaceID;

//Set default mapping ranges here.
float Mapper::pitchRange[2] = {12.0f, 24.0f};
float Mapper::LowPassQRange[2] = {0.1f, 2.9f};
float Mapper::LowPassCutoffRange[2] = {20.0f, 3000.0f};
float Mapper::HighPassQRange[2] = {0.1f, 2.9f};
float Mapper::HighPassCutoffRange[2] = {20.0f, 3000.0f};
float Mapper::BandPassQRange[2] = {0.1f, 2.9f};
float Mapper::BandPassCutoffRange[2] = {20.0f, 3000.0f};

int Mapper::releaseT = 1500;

void Mapper::routeParameters(int numFingers, bool isInPitchBar) // all the mapping are defined here, and the values updated for AudioParameters
{
    mapping.clear();
    AudioProcessorBundler::turnOffProcessors();
    
    switch (toggleSpaceID) {
        case 1: // sustain
            
            if (numFingers == 0)
            {

            }
            if (numFingers == 1)
            {
                selectPresetSustained(1, numFingers);   // SET PRESET HERE
                /*
                mapFromTo(X_POSITION, PITCH);
                mapFromTo(X_POSITION, BANDPASS_CUTOFF);
                mapFromTo(Y_POSITION, BANDPASS_Q);
                */
            }
            else if (numFingers >= 2)
            {
                selectPresetSustained(8, numFingers);   // SET PRESET HERE
                /*
                mapFromTo(Y_POSITION, PITCH);
                mapFromTo(X_POSITION, HIGHPASS_Q);
                 */
            }
            
            if (isInPitchBar)   //NO need for preset for the pitchbar. Change the range if needed
            {
                mapFromTo(Y_POSITION, DISCRETE_PITCH);
                setPitchRange(-12.0f, 24.0f);
            }
            break;
        case 2: // impulse
            selectPresetImpulse(6,numFingers);   //SET PRESET HERE
            /*
            setPitchRange(-4.0f, 12.0f);
            if (numFingers == 1)
            {
                mapFromTo(ABS_DIST, PITCH);
                mapFromTo(ABS_DIST, BANDPASS_CUTOFF);
                mapFromTo(ABS_DIST, BANDPASS_Q);
                mapFromTo(ABS_DIST, RELEASE);
                
            }
            else if (numFingers >= 2)
            {
                mapFromTo(ABS_DIST, PITCH);
                mapFromTo(ABS_DIST, HIGHPASS_CUTOFF);
                mapFromTo(ABS_DIST, RELEASE);
                
            }*/
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
    *AudioProcessorBundler::pitch = pitchRange[0] + val*pitchRange[1];
    AudioProcessorBundler::timeStretch->pitchUpdated = true;
}

void Mapper::mapToDiscretePitch(float val)
{
    *AudioProcessorBundler::pitch = 2 * floor ((val * pitchRange[1]/2) + pitchRange[0]/2);
    AudioProcessorBundler::timeStretch->pitchUpdated = true;
}

void Mapper::mapToTempo(float val)
{
    *AudioProcessorBundler::tempo = -50.0f + val*100.0f;
    AudioProcessorBundler::timeStretch->tempoUpdated = true;
}

void Mapper::mapToLowPassCutoff(float val)
{
    *AudioProcessorBundler::lowPassFilterFreqParam = LowPassCutoffRange[0] + val*LowPassCutoffRange[1] ;
}

void Mapper::mapToLowPassQ(float val)
{
    *AudioProcessorBundler::lowPassFilterQParam = LowPassQRange[0] + val*LowPassQRange[1];
}

void Mapper::mapToHighPassCutoff(float val)
{
    *AudioProcessorBundler::highPassFilterFreqParam = HighPassCutoffRange[0]  + val*HighPassCutoffRange[1] ;
    
}

void Mapper::mapToHighPassQ(float val)
{
    *AudioProcessorBundler::highPassFilterQParam = HighPassQRange[0] + val*HighPassQRange[1];
}

void Mapper::mapToBandPassCutoff(float val)
{
    *AudioProcessorBundler::bandPassFilterFreqParam = BandPassCutoffRange[0]  + val*BandPassCutoffRange[1] ;
}

void Mapper::mapCentroidToBandPassCutoff(float val)
{
    *AudioProcessorBundler::bandPassFilterFreqParam = + val;
}

void Mapper::mapToBandPassQ(float val)
{
    *AudioProcessorBundler::bandPassFilterQParam = BandPassQRange[0] + val*BandPassQRange[1];
}

void Mapper::mapToRelease(float val)
{
    releaseT = (int)(abs(val - 0.75)*1800)+1000;
}

void Mapper::mapToSustainedRelease(float val)
{
    releaseT = (int)(abs(val)*3000)+1000;
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
    AudioProcessorBundler::turnOffProcessors();
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
                        AudioProcessorBundler::turnOnProcessor(GAIN_ON);
                        break;
                    case PITCH:
                        //setPitchRange(-12.0f, 24.0f);
                        mapToPitch(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case DISCRETE_PITCH:
                        mapToDiscretePitch(Gesture::getFingerPosition(0).x);
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(TEMPO_ON);
                        break;
                    case LOWPASS_CUTOFF:
                        mapToLowPassCutoff(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case LOWPASS_Q:
                        mapToLowPassQ(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case HIGHPASS_CUTOFF:
                        mapToHighPassCutoff(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case HIGHPASS_Q:
                        mapToHighPassQ(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case BANDPASS_CUTOFF:
                        mapToBandPassCutoff(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case BANDPASS_Q:
                        mapToBandPassQ(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case RELEASE:
                        mapToRelease(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                    case SUSTAINED_RELEASE:
                        mapToSustainedRelease(Gesture::getFingerPosition(Gesture::getNumFingers()-1).x);
                        break;
                }
                break;
            case Y_POSITION:
                switch (audioParameter) {
                    case GAIN:
                        mapToGain(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(GAIN_ON);
                        break;
                    case PITCH:
                        mapToPitch(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case DISCRETE_PITCH:
                        mapToDiscretePitch(Gesture::getFingerPosition(0).y);
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(TEMPO_ON);
                        break;
                    case LOWPASS_CUTOFF:
                        mapToLowPassCutoff(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case LOWPASS_Q:
                        mapToLowPassQ(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case HIGHPASS_CUTOFF:
                        mapToHighPassCutoff(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case HIGHPASS_Q:
                        mapToHighPassQ(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case BANDPASS_CUTOFF:
                        mapToBandPassCutoff(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case BANDPASS_Q:
                        mapToBandPassQ(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case RELEASE:
                        mapToRelease(Gesture::getFingerPosition(Gesture::getNumFingers()-1).y);
                        break;
                    case SUSTAINED_RELEASE:
                        break;
                }
                break;
            case ABS_DIST:
                switch (audioParameter) {
                    case GAIN:
                        mapToGain(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(GAIN_ON);
                        break;
                    case PITCH:
                        mapToPitch(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case DISCRETE_PITCH:
                        /* ABS_DIST currently not mapped to DISCRETE_PITCH */
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(TEMPO_ON);
                        break;
                    case LOWPASS_CUTOFF:
                        mapToLowPassCutoff(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case LOWPASS_Q:
                        mapToLowPassQ(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case HIGHPASS_CUTOFF:
                        mapToHighPassCutoff(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case HIGHPASS_Q:
                        mapToHighPassQ(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case BANDPASS_CUTOFF:
                        mapToBandPassCutoff(2*Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case BANDPASS_Q:
                        mapToBandPassQ(Gesture::getAbsDistFromOrigin());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case RELEASE:
                        mapToRelease(Gesture::getAbsDistFromOrigin());
                        break;
                    case SUSTAINED_RELEASE:
                        break;
                }
                break;
            case PINCH_DIST:
                switch (audioParameter) {
                    case GAIN:
                        mapToGain(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(GAIN_ON);
                        break;
                    case PITCH:
                        mapToPitch(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case DISCRETE_PITCH:
                        /* ABS_DIST currently not mapped to DISCRETE_PITCH */
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(TEMPO_ON);
                        break;
                    case LOWPASS_CUTOFF:
                        mapToLowPassCutoff(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case LOWPASS_Q:
                        mapToLowPassQ(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case HIGHPASS_CUTOFF:
                        mapToHighPassCutoff(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case HIGHPASS_Q:
                        mapToHighPassQ(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case BANDPASS_CUTOFF:
                        mapToBandPassCutoff(2*Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case BANDPASS_Q:
                        mapToBandPassQ(Gesture::getDistBetweenFingers());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case RELEASE:
                        mapToRelease(Gesture::getDistBetweenFingers());
                        break;
                    case SUSTAINED_RELEASE:
                        break;
                }
                break;
            case VELOCITY:
                switch (audioParameter) {
                    case GAIN:
                        mapToGain(Gesture::getVelocity());
                        break;
                    case PITCH:
                        mapToPitch(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case DISCRETE_PITCH:
                        /* VELOCITY currently not mapped to DISCRETE_PITCH */
                        break;
                    case TEMPO:
                        mapToTempo(Gesture::getVelocity());
                        break;
                    case LOWPASS_CUTOFF:
                        mapToLowPassCutoff(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case LOWPASS_Q:
                        mapToLowPassQ(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(LOWPASS_ON);
                        break;
                    case HIGHPASS_CUTOFF:
                        mapToHighPassCutoff(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case HIGHPASS_Q:
                        mapToHighPassQ(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(HIGHPASS_ON);
                        break;
                    case BANDPASS_CUTOFF:
                        mapToBandPassCutoff(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case BANDPASS_Q:
                        mapToBandPassQ(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case RELEASE:
                        mapToRelease(Gesture::getVelocity());
                        break;
                    case SUSTAINED_RELEASE:
                        mapToSustainedRelease(Gesture::getVelocity());
                        break;
                }
                break;
            case CENTROID:
                switch (audioParameter) {
                    case GAIN:
                        break;
                    case PITCH:
                        break;
                    case DISCRETE_PITCH:
                        break;
                    case TEMPO:
                        break;
                    case LOWPASS_CUTOFF:
                        break;
                    case LOWPASS_Q:
                        break;
                    case HIGHPASS_CUTOFF:
                        break;
                    case HIGHPASS_Q:
                        break;
                    case BANDPASS_CUTOFF:
                        mapCentroidToBandPassCutoff(Gesture::getCentroid());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case BANDPASS_Q:
                        break;
                    case RELEASE:
                        break;
                    case SUSTAINED_RELEASE:
                        break;
                }
                break;
            case VELOCITY_MAX:
                switch (audioParameter) {
                    case GAIN:
                        break;
                    case PITCH:
                        mapToPitch (Gesture::getVelocityMax());
                        AudioProcessorBundler::turnOnProcessor(PITCH_ON);
                        break;
                    case DISCRETE_PITCH:
                        break;
                    case TEMPO:
                        break;
                    case LOWPASS_CUTOFF:
                        break;
                    case LOWPASS_Q:
                        break;
                    case HIGHPASS_CUTOFF:
                        break;
                    case HIGHPASS_Q:
                        break;
                    case BANDPASS_CUTOFF:
                        mapToBandPassCutoff(Gesture::getVelocity());
                        AudioProcessorBundler::turnOnProcessor(BANDPASS_ON);
                        break;
                    case BANDPASS_Q:
                        break;
                    case RELEASE:
                        break;
                    case SUSTAINED_RELEASE:
                        mapToSustainedRelease(Gesture::getVelocityMax());
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

void Mapper::setPitchRange(float min, float max)
{
    pitchRange[0] = min;
    pitchRange[1] = max;
}

void Mapper::selectPresetSustained(int index, int numFingers)
{
    switch(index){
        case 1:/*
                setPitchRange(0.0f, 4.00f);
                mapFromTo(VELOCITY, PITCH);*/
                mapFromTo(X_POSITION, BANDPASS_CUTOFF);
                mapFromTo(Y_POSITION, BANDPASS_Q);
                mapFromTo(VELOCITY, SUSTAINED_RELEASE);
            break;
        case 2:
                setPitchRange(-12.0f, 24.0f);
                mapFromTo(X_POSITION, PITCH);
                mapFromTo(X_POSITION, BANDPASS_CUTOFF);
                mapFromTo(Y_POSITION, BANDPASS_Q);
                mapFromTo(VELOCITY, SUSTAINED_RELEASE);
            break;
        case 3:
                setPitchRange(-6.0f, 12.0f);
                mapFromTo(X_POSITION, PITCH);
                mapFromTo(VELOCITY, SUSTAINED_RELEASE);
                mapFromTo(X_POSITION, LOWPASS_CUTOFF);
                mapFromTo(Y_POSITION, LOWPASS_Q);
            break;
        case 4:
                setPitchRange(-6.0f, 12.0f);
                mapFromTo(VELOCITY, PITCH);
                mapFromTo(VELOCITY, SUSTAINED_RELEASE);
                mapFromTo(X_POSITION, LOWPASS_CUTOFF);
                mapFromTo(Y_POSITION, LOWPASS_Q);
            break;
        case 5:
                setPitchRange(-6.0f, 12.0f);
                mapFromTo(VELOCITY, PITCH);
                mapFromTo(VELOCITY, SUSTAINED_RELEASE);
                mapFromTo(X_POSITION, HIGHPASS_CUTOFF);
                mapFromTo(Y_POSITION, HIGHPASS_Q);
            break;
        case 6:
                setPitchRange(-2.0f, 4.0f);
                mapFromTo(X_POSITION, PITCH);
                mapFromTo(CENTROID, BANDPASS_CUTOFF);
                mapFromTo(VELOCITY, BANDPASS_Q);
                mapFromTo(VELOCITY, SUSTAINED_RELEASE);
            break;
        case 7:
                mapFromTo(Y_POSITION, PITCH);
                mapFromTo(X_POSITION, HIGHPASS_Q);
            break;
        case 8:
                setPitchRange(-12.0f, 24.0f);
                mapFromTo(Y_POSITION, PITCH);
                mapFromTo(PINCH_DIST, LOWPASS_CUTOFF);
                mapFromTo(PINCH_DIST, LOWPASS_Q);
                mapFromTo(VELOCITY, SUSTAINED_RELEASE);
            break;
    }
}

void Mapper::selectPresetImpulse(int index, int numFingers)
{
    switch(index){
        case 1:
            setPitchRange(-4.0f, 12.0f);
            if (numFingers == 1)
            {
                mapFromTo(ABS_DIST, PITCH);
                mapFromTo(ABS_DIST, BANDPASS_CUTOFF);
                mapFromTo(ABS_DIST, BANDPASS_Q);
                mapFromTo(ABS_DIST, RELEASE);
            }
            break;
        case 2:
            setPitchRange(-4.0f, 12.0f);
            mapFromTo(ABS_DIST, PITCH);
            mapFromTo(ABS_DIST, BANDPASS_CUTOFF);
            mapFromTo(ABS_DIST, BANDPASS_Q);
            mapFromTo(ABS_DIST, RELEASE);
            break;
        case 3:
            setPitchRange(-12.0f, 24.0f);
            mapFromTo(ABS_DIST, PITCH);
            mapFromTo(ABS_DIST, BANDPASS_CUTOFF);
            mapFromTo(ABS_DIST, BANDPASS_Q);
            mapFromTo(ABS_DIST, RELEASE);
            break;
        case 4:
            setPitchRange(-4.0f, 12.0f);
            mapFromTo(ABS_DIST, PITCH);
            mapFromTo(ABS_DIST, HIGHPASS_CUTOFF);
            mapFromTo(ABS_DIST, HIGHPASS_Q);
            mapFromTo(ABS_DIST, RELEASE);
            break;
        case 5:
            setPitchRange(-12.0f, 24.0f);
            mapFromTo(ABS_DIST, PITCH);
            mapFromTo(ABS_DIST, HIGHPASS_CUTOFF);
            mapFromTo(ABS_DIST, HIGHPASS_Q);
            mapFromTo(ABS_DIST, RELEASE);
            break;
        case 6:
            setPitchRange(-4.0f, 12.0f);
            mapFromTo(ABS_DIST, PITCH);
            mapFromTo(CENTROID, BANDPASS_CUTOFF);
            mapFromTo(ABS_DIST, BANDPASS_Q);
            mapFromTo(ABS_DIST, RELEASE);
            break;
    }
}

std::vector< std::pair <GestureParameter,AudioParameter> > Mapper::mapping;
