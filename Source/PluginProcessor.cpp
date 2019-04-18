/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayEffectLab2AudioProcessor::DelayEffectLab2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        delayBuffer_(2,1)
#endif
{
    
    // set default values
    
    delayLength_ = 0.5;
    dryMix_ = 1.0;
    wetMix_ = 0.5;
    feedback_ = 0.75;
    delayBufferLength_ = 1;
    
    // start the circular buffer pointers at the beginning
    
    delayReadPosition_ = 0;
    delayWritePosition_ = 0;
    
}

DelayEffectLab2AudioProcessor::~DelayEffectLab2AudioProcessor()
{
}

//==============================================================================
const String DelayEffectLab2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayEffectLab2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayEffectLab2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayEffectLab2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayEffectLab2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayEffectLab2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayEffectLab2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayEffectLab2AudioProcessor::setCurrentProgram (int index)
{
}

const String DelayEffectLab2AudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayEffectLab2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

int DelayEffectLab2AudioProcessor::getNumParameters()
{
    return kNumParameters;
}

float DelayEffectLab2AudioProcessor::getParameter(int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    
    switch (index)
    {
        case kDryMixParam: return dryMix_;
        case kWetMixParam: return wetMix_;
        case kFeedbackParam: return feedback_;
        case kDelayLengthParam: return delayLength_;
        default: return 0.0f;
    }
}

const String DelayEffectLab2AudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case kDryMixParam: return "dry mix";
        case kWetMixParam: return "wet mix";
        case kFeedbackParam: return "feedback";
        case kDelayLengthParam:return "delay";
        default: break;
    }
    
    return String::empty;
}

void DelayEffectLab2AudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case kDryMixParam:
            dryMix_ = newValue;
            break;
        case kWetMixParam:
            wetMix_ = newValue;
            break;
        case kFeedbackParam:
            feedback_ = newValue;
            break;
        case kDelayLengthParam:
            delayLength_ = newValue;
            delayReadPosition_ = (int)(delayWritePosition_ - (delayLength_ * getSampleRate())
                                       + delayBufferLength_) % delayBufferLength_;
            break;
        default:
            break;
    }
}
//==============================================================================
void DelayEffectLab2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    delayBufferLength_ = (int) (2.0 * sampleRate);
    if(delayBufferLength_ < 1)
        delayBufferLength_ = 1;
    delayBuffer_.setSize(2, delayBufferLength_);
    delayBuffer_.clear();
    
    // this method gives us the sample rate. Use this to figure out what the delay position offset should be.
    // Since it is specified in seconds, we convert it to number of samples
    
    delayReadPosition_ = (int)(delayWritePosition_ - (delayLength_ * getSampleRate()) + delayBufferLength_) % delayBufferLength_;
        
}

void DelayEffectLab2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayEffectLab2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DelayEffectLab2AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    const int numSamples = buffer.getNumSamples(); //how many samples in the buffer
    
    int dpr, dpw; //dpr = delay read pointer, dpw = delay write pointer
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //channel data is an array of length numSamples which contains the audio for one channel
        float* channelData = buffer.getWritePointer (channel);
        
        // delay data is the circular buffer for implementing delay on this channel
        
        float* delayData = delayBuffer_.getWritePointer(jmin(channel, delayBuffer_.getNumChannels() - 1));
        
        //make a temporary copy of any state variables declared in PluginProcessor.h which need to be
        //maintained between calls to processBlock(). Each channel needs to be processed identically
        //which means that the activity of processing one channel can't affect the state variable for
        //the next channel.
        
        dpr = delayReadPosition_;
        dpw = delayWritePosition_;
        
        for(int i = 0; i < numSamples; i++)
        {
            const float in = channelData[i];
            float out = 0.0;
            
            //here the output is the input plus the contents of the delay buffer (weighted by delayMix)
            
            out = (dryMix_ * in + wetMix_ * delayData[dpr]);
            
            // Store the current information in the delay buffer. delayData[dpr] is the delay sample we
            //just read, i.e. what came out of the buffer. delayData[dpw] is what we write to the buffer
            
            delayData[dpw] = in + (delayData[dpr] * feedback_);
            
            if(++dpr >= delayBufferLength_)
                dpr = 0;
            if(++dpw >= delayBufferLength_)
                dpw = 0;
            
            // store the output sample in the buffer, replacing the input
            
            channelData[i] = out;
        }
        
        
    }
    
    // Having made a local copy of the state variables for each channel, now transfer the result
    //back to the main state variable so they will be preserved for the next call of processBlock()
    
    delayReadPosition_ = dpr;
    delayWritePosition_ = dpw;
    
}

//==============================================================================
bool DelayEffectLab2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayEffectLab2AudioProcessor::createEditor()
{
    return new DelayEffectLab2AudioProcessorEditor (*this);
}

//==============================================================================
void DelayEffectLab2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayEffectLab2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayEffectLab2AudioProcessor();
}
