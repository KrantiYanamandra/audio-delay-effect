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
class DelayEffectLab2AudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public Timer
{
public:
    DelayEffectLab2AudioProcessorEditor (DelayEffectLab2AudioProcessor&);
    ~DelayEffectLab2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void sliderValueChanged (Slider*) override;
    void timerCallback() override;
    
private:
    
    Label delayLengthLabel_, feedbackLabel_, dryMixLabel_, wetMixLabel_;
    Slider delayLengthSlider_, feedbackSlider_, dryMixSlider_, wetMixSlider_;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayEffectLab2AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayEffectLab2AudioProcessorEditor)
};
