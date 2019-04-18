/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayEffectLab2AudioProcessorEditor::DelayEffectLab2AudioProcessorEditor (DelayEffectLab2AudioProcessor& p)
: AudioProcessorEditor (&p),
delayLengthLabel_("", "Delay (s):"),
feedbackLabel_("", "Feedback:"),
dryMixLabel_("", "Dry Level:"),
wetMixLabel_("", "Wet Level:"),
processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (450, 150);
    
    //setting up the sliders
    
    addAndMakeVisible(&delayLengthSlider_);
    delayLengthSlider_.setSliderStyle(Slider::Rotary);
    delayLengthSlider_.addListener(this);
    delayLengthSlider_.setRange(0.01, 2.0, 0.01);
    delayLengthSlider_.setValue(processor.getParameter(DelayEffectLab2AudioProcessor::kDelayLengthParam));
    
    
    addAndMakeVisible(&feedbackSlider_);
    feedbackSlider_.setSliderStyle(Slider::Rotary);
    feedbackSlider_.addListener(this);
    feedbackSlider_.setRange(0.0, 0.995, 0.005);
    feedbackSlider_.setValue(processor.getParameter(DelayEffectLab2AudioProcessor::kFeedbackParam));
    
    addAndMakeVisible(&dryMixSlider_);
    dryMixSlider_.setSliderStyle(Slider::Rotary);
    dryMixSlider_.addListener(this);
    dryMixSlider_.setRange(0.0, 1.0, 0.01);
    dryMixSlider_.setValue(processor.getParameter(DelayEffectLab2AudioProcessor::kDryMixParam));
    
    addAndMakeVisible(&wetMixSlider_);
    wetMixSlider_.setSliderStyle(Slider::Rotary);
    wetMixSlider_.setRange(0.0, 1.0, 0.01);
    wetMixSlider_.setValue(processor.getParameter(DelayEffectLab2AudioProcessor::kWetMixParam));
    
    delayLengthLabel_.attachToComponent(&delayLengthSlider_, true);
    
    feedbackLabel_.attachToComponent(&feedbackSlider_, true);
    
    dryMixLabel_.attachToComponent(&dryMixSlider_, true);
    
    wetMixLabel_.attachToComponent(&wetMixSlider_, true);
    
}

// This timer periodically (every 50 millisecinds) checks whether any of the filter's parameters have changed

void DelayEffectLab2AudioProcessorEditor::timerCallback()
{
    
    delayLengthSlider_.setValue(processor.delayLength_, dontSendNotification);
    feedbackSlider_.setValue(processor.feedback_, dontSendNotification);
    dryMixSlider_.setValue(processor.dryMix_, dontSendNotification);
    wetMixSlider_.setValue(processor.wetMix_, dontSendNotification);
    
}

// This is the slider listener callback when user drags a slider

void DelayEffectLab2AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    //Its vital to use setParameterNotifyingHost to change any parameters that are automatable by the host, rather than just modifying them directly. Otherwise,the host wont know that they have changed.
    
    if (slider == &delayLengthSlider_)
    {
        processor.setParameterNotifyingHost(DelayEffectLab2AudioProcessor::kDelayLengthParam, (float) delayLengthSlider_.getValue());
    }
    
    else if (slider == &feedbackSlider_)
    {
        processor.setParameterNotifyingHost(DelayEffectLab2AudioProcessor::kFeedbackParam, (float) feedbackSlider_.getValue());
    }
    
    else if (slider == &dryMixSlider_)
    {
        processor.setParameterNotifyingHost(DelayEffectLab2AudioProcessor::kDryMixParam, (float) dryMixSlider_.getValue());
    }
    
    else if (slider == &wetMixSlider_)
    {
        processor.setParameterNotifyingHost(DelayEffectLab2AudioProcessor::kWetMixParam, (float) wetMixSlider_.getValue());
    }
}

DelayEffectLab2AudioProcessorEditor::~DelayEffectLab2AudioProcessorEditor()
{
}

//==============================================================================
void DelayEffectLab2AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DelayEffectLab2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    delayLengthSlider_.setBounds(60, 10, 150, 75);
    feedbackSlider_.setBounds(280, 10, 150, 75);
    dryMixSlider_.setBounds(60, 70, 150, 75);
    wetMixSlider_.setBounds(280, 70, 150, 75);
    
}
