#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ID_APVTS.h"

AdditiveSynthesisAudioProcessor::AdditiveSynthesisAudioProcessor() :
    #ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true)),
    #endif
    apvts (*this, nullptr, Identifier (ID_APVTS), {
            std::make_unique<AudioParameterFloat> (ID_GAIN0,
                                                  "Gain 0",
                                                   0.0f,
                                                   1.0f,
                                                   0.5f),
            std::make_unique<AudioParameterFloat> (ID_GAIN1,
                                                  "Gain 1",
                                                   0.0f,
                                                   1.0f,
                                                   0.0f),
            std::make_unique<AudioParameterFloat> (ID_GAIN2,
                                                  "Gain 2",
                                                   0.0f,
                                                   1.0f,
                                                   0.0f),
            std::make_unique<AudioParameterFloat> (ID_GAIN3,
                                                  "Gain 3",
                                                   0.0f,
                                                   1.0f,
                                                   0.0f),
            std::make_unique<AudioParameterFloat> (ID_MASTER,
                                                  "Master",
                                                   0.0f,
                                                   1.0f,
                                                   0.5f),
            std::make_unique<AudioParameterFloat> (ID_FREQ1,
                                                    "Freq Osc 1",
                                                    NormalisableRange<float> (0.5f, +2.0f, 0.01f),
                                                    1.0f),
            std::make_unique<AudioParameterFloat> (ID_FREQ2,
                                                    "Freq Osc 2",
                                                    NormalisableRange<float> (0.5f, +2.0f, 0.01f),
                                                    1.0f),
            std::make_unique<AudioParameterFloat> (ID_FREQ3,
                                                    "Freq Osc 3",
                                                    NormalisableRange<float> (0.5f, +2.0f, 0.01f),
                                                    1.0f),
            std::make_unique<AudioParameterFloat> (ID_ATK,
                                                   "Attack",
                                                   0.01f,
                                                   2.00f,
                                                   0.10f),
            std::make_unique<AudioParameterFloat> (ID_DEC,
                                                   "Decay",
                                                   0.01f,
                                                   1.00f,
                                                   0.10f),
            std::make_unique<AudioParameterFloat> (ID_SUS,
                                                   "Sustain",
                                                   0.00f,
                                                   1.00f,
                                                   1.00f),
            std::make_unique<AudioParameterFloat> (ID_REL,
                                                   "Release",
                                                   0.01f,
                                                   2.00f,
                                                   0.10f),
            std::make_unique<AudioParameterFloat> (ID_REVSIZE,
                                                   "Reverb Amount",
                                                   0.00f,
                                                   1.00f,
                                                   0.50f),
            std::make_unique<AudioParameterFloat> (ID_REVDRWE,
                                                   "Reverb DryWet",
                                                   0.00f,
                                                   1.00f,
                                                   0.00f),
        })
{
    /// Parameters retrivial
    masterParameter    = apvts.getRawParameterValue(ID_MASTER);
    revSizeParameter   = apvts.getRawParameterValue(ID_REVSIZE);
    revDryWetParameter = apvts.getRawParameterValue(ID_REVDRWE);
    
    /// Synth Initialisation
    auto maxNum = 4;
    for (int i = 0; i < maxNum; i++)
    {
        synth.addVoice (new SynthVoice(apvts));
    }
    synth.addSound (new SinSound);
    
    /// Reverb Initialisation
    reverb.setParameters ({
               *revSizeParameter,
                            0.5f,
             *revDryWetParameter,
      1.0f - *revDryWetParameter,
                            1.0f,
                            0.0f,
    });
    
    /// Master Attribute init
    smoothMaster = *masterParameter;
}


AdditiveSynthesisAudioProcessor::~AdditiveSynthesisAudioProcessor() {}


void AdditiveSynthesisAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    reverb.setSampleRate (sampleRate);
}


void AdditiveSynthesisAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    /// Buffer clearing
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    /// Synthesiser Rendering
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    /// Reverb Rendering
    reverb.setParameters ({
               *revSizeParameter,
                            0.5f,
             *revDryWetParameter,
      1.0f - *revDryWetParameter,
                            1.0f,
                            0.0f,
    });
    reverb.processStereo (buffer.getWritePointer (0), buffer.getWritePointer (1), buffer.getNumSamples());
    
    /// Gain stage
    smoothMaster = smoothMaster - 0.5 * (smoothMaster - *masterParameter);
    buffer.applyGain (smoothMaster); /// Basic implementation, should be smoother with
}



//===========================================================================================

const String AdditiveSynthesisAudioProcessor::getName() const { return JucePlugin_Name; }

bool AdditiveSynthesisAudioProcessor::acceptsMidi() const { return true; }

bool AdditiveSynthesisAudioProcessor::producesMidi() const { return false; }

bool AdditiveSynthesisAudioProcessor::isMidiEffect() const { return false; }

double AdditiveSynthesisAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AdditiveSynthesisAudioProcessor::getNumPrograms() { return 1; }

int AdditiveSynthesisAudioProcessor::getCurrentProgram() { return 0; }

void AdditiveSynthesisAudioProcessor::setCurrentProgram (int index) {}

const String AdditiveSynthesisAudioProcessor::getProgramName (int index) { return {}; }

void AdditiveSynthesisAudioProcessor::changeProgramName (int index, const String& newName) {}

void AdditiveSynthesisAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AdditiveSynthesisAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    return true;
}
#endif

bool AdditiveSynthesisAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor* AdditiveSynthesisAudioProcessor::createEditor() { return new AdditiveSynthesisAudioProcessorEditor (*this); }

void AdditiveSynthesisAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AdditiveSynthesisAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
       if (xmlState->hasTagName (apvts.state.getType()))
           apvts.replaceState (ValueTree::fromXml (*xmlState));
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new AdditiveSynthesisAudioProcessor(); }
