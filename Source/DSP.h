/*
  ==============================================================================

    DSP.h
    Created: 20 Oct 2017 12:54:24pm
    Authors: Michael Castanieto
             Gergely Csapo
             Jonas Holfelt
             
    Description: This is the base class for the DSP Library. All DSP classes that
                 inherit from the DSP base class will function as AudioProcessors
                 with a 'state' for holding the current state of the DSP object
                 and a 'process' function for processing real-time audio

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"	

class DSP : public AudioProcessor
{
	public:
		DSP();
		~DSP();
		/** these methods must override AudioProcessor **/
		const String getName() const override;
		void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock) override;
		void releaseResources() override;
		void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
		const String getInputChannelName(int channelIndex) const override;
		const String getOutputChannelName(int channelIndex) const override;
		bool isInputChannelStereoPair(int index) const override;
		bool isOutputChannelStereoPair(int index) const override;
		bool silenceInProducesSilenceOut() const override;
		double getTailLengthSeconds() const override;
		bool acceptsMidi() const override;
		bool producesMidi() const override;
		AudioProcessorEditor* createEditor() override;
		bool hasEditor() const override;
		int getNumPrograms() override;
		int getCurrentProgram() override;
		void setCurrentProgram(int index) override;
		const String getProgramName(int index) override;
		void changeProgramName(int index, const String& newName) override;
		void getStateInformation (juce::MemoryBlock& destData) override;
		void setStateInformation (const void *data, int sizeInBytes) override;
		/* =========================================================================== */
		/* pure virtual function, the DSP class that inherits
		   this must implement this function to process a signal */
    	virtual void process(AudioBuffer<float> buffer) = 0; 

	protected:
		float **state; // holds the current state of the DSP object

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSP);
};