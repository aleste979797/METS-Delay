/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SliderLookAndFeel : public LookAndFeel_V4
{
public:
    
    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPos,
                           float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override
    {
        
        //Create Varible for entire Knob.
        knobMaster = ImageCache::getFromMemory(BinaryData::knobMaster_png ,BinaryData::knobMaster_pngSize);
        
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

        //Draw Knob.
        //knobMaster.getWidth() /2, knobMaster.getHeight() /2 Changes the point of rotation.
        g.drawImageTransformed(knobMaster, AffineTransform::rotation(angle, knobMaster.getWidth() /2, knobMaster.getHeight() /2));
        
    }
private:
//Image del knob.
Image knobMaster;
};

class MetsDelayReworkedAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    MetsDelayReworkedAudioProcessorEditor (MetsDelayReworkedAudioProcessor&);
    ~MetsDelayReworkedAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Slider mDryWetSlider;
    Label drywet;
    Slider mFeedbackSlider;
    Label feedback;
    Slider mDelayTimeSlider;
    Label delaytime;
    std::unique_ptr<HyperlinkButton> hyperlinkButton;
    Image backgroundImage;
    SliderLookAndFeel sliderLookAndFeel;

    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MetsDelayReworkedAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetsDelayReworkedAudioProcessorEditor)
};
