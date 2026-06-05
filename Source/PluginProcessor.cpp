/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
FAPPAudioProcessor::FAPPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

FAPPAudioProcessor::~FAPPAudioProcessor()
{
}

//==============================================================================
const juce::String FAPPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FAPPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FAPPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FAPPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FAPPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FAPPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FAPPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FAPPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FAPPAudioProcessor::getProgramName (int index)
{
    return {};
}

void FAPPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FAPPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dspCore.prepare(sampleRate);
}

void FAPPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FAPPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void FAPPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    float filterType = *apvts.getRawParameterValue("TYPE");
    float freq = *apvts.getRawParameterValue("FREQ");
    float q = *apvts.getRawParameterValue("Q");
    int iterations = (int)*apvts.getRawParameterValue("ITER");
    float mix = *apvts.getRawParameterValue("MIX");

    auto channelData = buffer.getArrayOfWritePointers();
    int numChannels = buffer.getNumChannels();
    int numSamples = buffer.getNumSamples();

    dspCore.setConfig(filterType > 0.5f, iterations);
    dspCore.updateCoefficients(freq, q, numChannels);
    dspCore.processBlock(channelData, numChannels, numSamples);
}

bool FAPPAudioProcessor::hasEditor() const
{
    return false;
}

juce::AudioProcessorEditor* FAPPAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void FAPPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FAPPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FAPPAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FAPPAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Filter Type (Biquad / SVF)
    params.push_back(std::make_unique<juce::AudioParameterChoice>("TYPE", "Filter Type", juce::StringArray{"Biquad", "SVF"}, 0));
    // Frequency (20Hz ~ 20000Hz)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ", "Frequency", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 200.0f));
    // Q (0.005 ~ 1.414)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Q", "Q", juce::NormalisableRange<float>(0.005f, 1.414f, 0.001f), 0.707f));
    // Iterations (0 ~ 50)
    params.push_back(std::make_unique<juce::AudioParameterInt>("ITER", "Iterations", 0, 50, 25));
    // Mix (0% ~ 100%)
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));

    return { params.begin(), params.end() };
}