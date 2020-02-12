/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MetsDelayReworkedAudioProcessorEditor::MetsDelayReworkedAudioProcessorEditor (MetsDelayReworkedAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    backgroundImage = ImageCache::getFromMemory(BinaryData::Sfondo_png, BinaryData::Sfondo_pngSize);
    
    setSize (600, 300);
    
    auto& params = processor.getParameters();
    //1
    AudioParameterFloat* DryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);
    
    mDryWetSlider.setBounds(170, 150, 60, 100);
    mDryWetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mDryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    mDryWetSlider.setRange(DryWetParameter->range.start, DryWetParameter->range.end, 0.01);
    mDryWetSlider.setValue(*DryWetParameter);
    mDryWetSlider.setLookAndFeel(&sliderLookAndFeel);
    mDryWetSlider.setTextValueSuffix("%");
    addAndMakeVisible(mDryWetSlider);
    drywet.setText("Dry / Wet", dontSendNotification);
    drywet.attachToComponent(&mDryWetSlider, false);
    drywet.setColour (Label::textColourId, Colours::white);
    drywet.setJustificationType(Justification::top);
    
    mDryWetSlider.onValueChange = [this, DryWetParameter] { *DryWetParameter = mDryWetSlider.getValue(); };
    mDryWetSlider.onDragStart = [DryWetParameter] {DryWetParameter-> beginChangeGesture(); };
    mDryWetSlider.onDragEnd = [DryWetParameter] {DryWetParameter-> endChangeGesture(); };
    
    //2
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*)params.getUnchecked(1);
    
    mFeedbackSlider.setBounds(370, 150, 60, 100);
    mFeedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mFeedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 55, 20);
    mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end, 0.01);
    mFeedbackSlider.setValue(*feedbackParameter);
    mFeedbackSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(mFeedbackSlider);
    addAndMakeVisible(feedback);
    feedback.setText("Feedback", dontSendNotification);
    feedback.attachToComponent(&mFeedbackSlider, false);
    feedback.setColour (Label::textColourId, Colours::white);
    feedback.setJustificationType(Justification::top);
    
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue(); };
    mFeedbackSlider.onDragStart = [feedbackParameter] {feedbackParameter-> beginChangeGesture(); };
    mFeedbackSlider.onDragEnd = [feedbackParameter] {feedbackParameter-> endChangeGesture(); };
    
    //3
    AudioParameterFloat* delayTimeParameter = (AudioParameterFloat*)params.getUnchecked(2);

    mDelayTimeSlider.setBounds(270, 150, 60, 100);
    mDelayTimeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mDelayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 65, 20);
    mDelayTimeSlider.setRange(delayTimeParameter->range.start, delayTimeParameter->range.end, 0.01);
    mDelayTimeSlider.setValue(*delayTimeParameter);
    mDelayTimeSlider.setTextValueSuffix("Sec");
    mDelayTimeSlider.setLookAndFeel(&sliderLookAndFeel);
    addAndMakeVisible(mDelayTimeSlider);
    addAndMakeVisible(mFeedbackSlider);
    addAndMakeVisible(delaytime);
    delaytime.setText("Delay Time", dontSendNotification);
    delaytime.attachToComponent(&mDelayTimeSlider, false);
    delaytime.setColour (Label::textColourId, Colours::white);
    delaytime.setJustificationType(Justification::top);

    mDelayTimeSlider.onValueChange = [this, delayTimeParameter] { *delayTimeParameter = mDelayTimeSlider.getValue(); };
    mDelayTimeSlider.onDragStart = [delayTimeParameter] {delayTimeParameter-> beginChangeGesture(); };
    mDelayTimeSlider.onDragEnd = [delayTimeParameter] {delayTimeParameter-> endChangeGesture(); };
    
    
    hyperlinkButton.reset (new HyperlinkButton (String(),
                                                URL ("https://metscuneo.it")));
    addAndMakeVisible (hyperlinkButton.get());
    hyperlinkButton->setTooltip (TRANS("www.metscuneo.it"));
    hyperlinkButton->setButtonText (String());

    hyperlinkButton->setBounds (150, 10, 300, 50);

}



MetsDelayReworkedAudioProcessorEditor::~MetsDelayReworkedAudioProcessorEditor()
{
}

//==============================================================================
void MetsDelayReworkedAudioProcessorEditor::paint (Graphics& g)
{
    g.drawImage(backgroundImage, 0, 0, 600, 300, 0, 0, 2043, 1169);
}

void MetsDelayReworkedAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
