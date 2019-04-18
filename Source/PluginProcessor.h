/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class DelayEffectLab2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayEffectLab2AudioProcessor();
    ~DelayEffectLab2AudioProcessor();

    int getNumParameters() override;
    
    float getParameter (int index) override;
    void setParameter (int index, float newVaue) override;
    
    const String getParameterName (int index) override;
    
    enum parameters
    {
        kDelayLengthParam = 0,
        kDryMixParam,
        kWetMixParam,
        kFeedbackParam,
        kNumParameters
    };
    
    //Adjustable Parameters
    
    float delayLength_; //length of delay line in seconds
    float dryMix_;      //Mix level of original signal (0-1)
    float wetMix_;      //Mix level of delayed signal (0-1)
    float feedback_;    //Feedback level (o - just less than 1)
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
    //circular buffer variables for implementing delay
    
    AudioSampleBuffer delayBuffer_;
    int delayBufferLength_;
    int delayReadPosition_, delayWritePosition_;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayEffectLab2AudioProcessor)
};
