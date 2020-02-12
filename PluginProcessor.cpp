/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MetsDelayReworkedAudioProcessor::MetsDelayReworkedAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet",
                                                            "DryWet",
                                                            0.0,
                                                            1.0,
                                                            0.5));
    
    addParameter(mFeedbackParameter = new AudioParameterFloat("feedback",
                                                              "Feedback",
                                                              0,
                                                              0.98,
                                                              0.5));
    
    addParameter(mDelayTimeParameter = new AudioParameterFloat("delaytime",
                                                               "DelayTime",
                                                               0.00,
                                                               MAX_DELAY_TIME,
                                                               0.5));
    
    
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    mDelayTimeInSamples = 0;
    mDelayReadHead = 0;
    
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
    mDelayTimeSmoothed = 0;

}

MetsDelayReworkedAudioProcessor::~MetsDelayReworkedAudioProcessor()
{
    if (mCircularBufferLeft != nullptr)
    {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight != nullptr)
    {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;

    }
}

//==============================================================================
const String MetsDelayReworkedAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MetsDelayReworkedAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MetsDelayReworkedAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MetsDelayReworkedAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MetsDelayReworkedAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MetsDelayReworkedAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MetsDelayReworkedAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MetsDelayReworkedAudioProcessor::setCurrentProgram (int index)
{
}

const String MetsDelayReworkedAudioProcessor::getProgramName (int index)
{
    return {};
}

void MetsDelayReworkedAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MetsDelayReworkedAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    mDelayTimeInSamples = sampleRate * *mDelayTimeParameter;
    
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
    
    if (mCircularBufferLeft == nullptr)
    {
        mCircularBufferLeft = new float[mCircularBufferLength];
    }
    zeromem (mCircularBufferLeft, mCircularBufferLength * sizeof(float));
    
    if (mCircularBufferRight == nullptr)
    {
        mCircularBufferRight = new float[mCircularBufferLength];
    }
    zeromem (mCircularBufferRight, mCircularBufferLength * sizeof(float));

    
    mCircularBufferWriteHead = 0;
    
    mDelayTimeSmoothed = *mDelayTimeParameter;
    
    
}

void MetsDelayReworkedAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MetsDelayReworkedAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MetsDelayReworkedAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    


    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);

    
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        
        mDelayTimeSmoothed = mDelayTimeSmoothed - 0.001 * (mDelayTimeSmoothed - *mDelayTimeParameter);
        
        mDelayTimeInSamples = getSampleRate() * mDelayTimeSmoothed;
        
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        mDelayReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        if (mDelayReadHead < 0)
        {
            mDelayReadHead += mCircularBufferLength;
        }
        
        // separo gli int dai float per l'interpolazione
        int readHead_x = (int)mDelayReadHead; //fase di x
        int readHead_x1 = readHead_x + 1; //fase di x1
        float readHeadFloat = mDelayReadHead - readHead_x; //fase del valore intermedio

        if (readHead_x >= mCircularBufferLength)
        {
            readHead_x1 -= mCircularBufferLength;
        }
    
        float delay_sample_left = lin_interp(mCircularBufferLeft[readHead_x], mCircularBufferLeft[readHead_x1], readHeadFloat);
        float delay_sample_right = lin_interp(mCircularBufferRight[readHead_x], mCircularBufferRight[readHead_x1], readHeadFloat);
        
        mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
        mFeedbackRight = delay_sample_right * *mFeedbackParameter;

        mCircularBufferWriteHead++;
        // agginta sample da ignorare
       /* buffer.addSample(0, i, delay_sample_left);
        buffer.addSample(1, i, delay_sample_right);*/
        
        buffer.setSample(0, i, buffer.getSample(0, i) * (1 - *mDryWetParameter) + delay_sample_left * *mDryWetParameter);
        buffer.setSample(1, i, buffer.getSample(1, i) * (1 - *mDryWetParameter) + delay_sample_right * *mDryWetParameter);
        
        if (mCircularBufferWriteHead >= mCircularBufferLength)
        {
            mCircularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool MetsDelayReworkedAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* MetsDelayReworkedAudioProcessor::createEditor()
{
    return new MetsDelayReworkedAudioProcessorEditor (*this);
}

//==============================================================================
void MetsDelayReworkedAudioProcessor::getStateInformation (MemoryBlock& destData)
{

    std::unique_ptr<XmlElement> xml(new XmlElement("DelayParams"));
    xml->setAttribute("DryWet", *mDryWetParameter);
    xml->setAttribute("Feedback", *mFeedbackParameter);
    xml->setAttribute("Delaytime", *mDelayTimeParameter);
    copyXmlToBinary(*xml, destData);
    
    /*
    MemoryOutputStream stream (destData, true);

    stream.writeFloat (*mDryWetParameter);
    stream.writeFloat (*mFeedbackParameter);
    stream.writeFloat (*mDelayTimeParameter);*/

}

void MetsDelayReworkedAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName("DelayParams"))
    {
        *mDryWetParameter = xml->getDoubleAttribute("DryWet");
        *mFeedbackParameter = xml->getDoubleAttribute("Feedback");
        *mDelayTimeParameter = xml->getDoubleAttribute("Delaytime");

        
    }
    /*
    MemoryInputStream stream (data, static_cast<size_t> (sizeInBytes), false);

    mDryWetParameter->setValueNotifyingHost (stream.readFloat());
    mFeedbackParameter->setValueNotifyingHost (stream.readFloat());
    mDelayTimeParameter->setValueNotifyingHost (stream.readFloat());*/
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MetsDelayReworkedAudioProcessor();
}

float MetsDelayReworkedAudioProcessor::lin_interp(float sample_x, float sample_x1, float inPhase)
{
    return (1 - inPhase) * sample_x + inPhase * sample_x1;
}
