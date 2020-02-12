/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define MAX_DELAY_TIME 2.0f
#define DRYWET_ID "drywet"
#define DRYWET_NAME "Drywet"
#define FEEDBACK_ID "feedback"
#define FEEDBACK_NAME "Feedback"
#define DELAYTIME_ID "delaytime"
#define DELAYTIME_NAME "Delaytime"

//==============================================================================
/**
*/
class MetsDelayReworkedAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    MetsDelayReworkedAudioProcessor();
    ~MetsDelayReworkedAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
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

    float lin_interp(float sample_x, float sample_x1, float inPhase);
private:
    
    float mDelayTimeSmoothed; 
    AudioParameterFloat* mDryWetParameter;
    AudioParameterFloat* mFeedbackParameter;
    AudioParameterFloat* mDelayTimeParameter;
    
    
    float mFeedbackRight;
    float mFeedbackLeft;
    
    float mDelayTimeInSamples;
    float mDelayReadHead;
    
    int mCircularBufferWriteHead;
    int mCircularBufferLength;
    
    float* mCircularBufferLeft;
    float* mCircularBufferRight;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetsDelayReworkedAudioProcessor)
};
