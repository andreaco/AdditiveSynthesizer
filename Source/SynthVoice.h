#pragma once
#include <JuceHeader.h>
#include "ID_APVTS.h"

///SynthesiserSound to be used by the SynthesiserVoice class
class SinSound : public SynthesiserSound
{
public:
    SinSound() {}
    bool appliesToNote (int)    override { return true; }
    bool appliesToChannel (int) override { return true; }
};


///SynthesiserVoice class
class SynthVoice : public SynthesiserVoice
{
public: 
    
    SynthVoice (AudioProcessorValueTreeState& apvts)
    {
        /// Retrivial of parameters from APVTS
        gain0Parameter = apvts.getRawParameterValue (ID_GAIN0);
        gain1Parameter = apvts.getRawParameterValue (ID_GAIN1);
        gain2Parameter = apvts.getRawParameterValue (ID_GAIN2);
        gain3Parameter = apvts.getRawParameterValue (ID_GAIN3);

        freq1Parameter = apvts.getRawParameterValue (ID_FREQ1);
        freq2Parameter = apvts.getRawParameterValue (ID_FREQ2);
        freq3Parameter = apvts.getRawParameterValue (ID_FREQ3);

        attackParameter  = apvts.getRawParameterValue (ID_ATK);
        decayParameter   = apvts.getRawParameterValue (ID_DEC);
        sustainParameter = apvts.getRawParameterValue (ID_SUS);
        releaseParameter = apvts.getRawParameterValue (ID_REL);
        
        adsr.setSampleRate(getSampleRate());
    };

    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int) override
    {
        /// Frequency reset
        freq0 = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        smoothFreq0 = freq0;
        smoothFreq1 = freq0 * *freq1Parameter;
        smoothFreq2 = freq0 * *freq2Parameter;
        smoothFreq3 = freq0 * *freq3Parameter;
        
        /// Phase reset
        phase0 = 0;
        phase1 = 0;
        phase2 = 0;
        phase3 = 0;
        
        /// We set the ADSR parameters only when the note is started
        adsr.setParameters ({
           *attackParameter,
           *decayParameter,
           *sustainParameter,
           *releaseParameter 
        });
        adsr.noteOn();
    }
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        adsr.noteOff();
    }
    
    void pitchWheelMoved (int newPitchWheelValue) override {}
    
    void controllerMoved (int controllerNumber, int newControllerValue) override {}
   
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        while (--numSamples >= 0) {
            /// Frequency Parameter Smoothing (to avoid clicks)
            smoothFreq0 = smoothing (smoothFreq0, freq0, 0.0005);
            smoothFreq1 = smoothing (smoothFreq1, *freq1Parameter * smoothFreq0, 0.0005);
            smoothFreq2 = smoothing (smoothFreq2, *freq2Parameter * smoothFreq0, 0.0005);
            smoothFreq3 = smoothing (smoothFreq3, *freq3Parameter * smoothFreq0, 0.0005);
            
            /// Gain Parameter Smoothing (to avoid clicks)
            smoothGain0 = smoothing (smoothGain0, *gain0Parameter, 0.0005);
            smoothGain1 = smoothing (smoothGain1, *gain1Parameter, 0.0005);
            smoothGain2 = smoothing (smoothGain2, *gain2Parameter, 0.0005);
            smoothGain3 = smoothing (smoothGain3, *gain3Parameter, 0.0005);
            
            /// Oscillator Computation
            auto osc0 = sin (phase0) * smoothGain0;
            auto osc1 = sin (phase1) * smoothGain1;
            auto osc2 = sin (phase2) * smoothGain2;
            auto osc3 = sin (phase3) * smoothGain3;
            
            /// Buffer Writing
            auto normVal = 0.2f;
            auto currentSample = (osc0 + osc1 + osc2 + osc3) * normVal * adsr.getNextSample();
            for (auto c = outputBuffer.getNumChannels() - 1; c >= 0; c--)
            {
                outputBuffer.addSample(c, startSample, currentSample);
            }

            /// Phase offset computation
            phase0 += 2.0f * MathConstants<double>::pi * smoothFreq0  / getSampleRate();
            phase1 += 2.0f * MathConstants<double>::pi * smoothFreq1  / getSampleRate();
            phase2 += 2.0f * MathConstants<double>::pi * smoothFreq2  / getSampleRate();
            phase3 += 2.0f * MathConstants<double>::pi * smoothFreq3  / getSampleRate();
            
            /// Phase reset if needed
            if (phase0 >= 2 * MathConstants<double>::pi) phase0 -= 2 * MathConstants<double>::pi;
            if (phase1 >= 2 * MathConstants<double>::pi) phase1 -= 2 * MathConstants<double>::pi;
            if (phase2 >= 2 * MathConstants<double>::pi) phase2 -= 2 * MathConstants<double>::pi;
            if (phase3 >= 2 * MathConstants<double>::pi) phase3 -= 2 * MathConstants<double>::pi;
            
            startSample++;
        }
    }
    
    using SynthesiserVoice::renderNextBlock;
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SinSound*> (sound) != nullptr;
    }
    
    /// Simple smoothing function
    inline float smoothing (float parameter, float target, float scalar)
    {
        return parameter - scalar * (parameter - target);
    }
    
private:
    
    /// Freq Attributes
    float freq0 = 0;
    float smoothFreq0 = 0;
    float smoothFreq1 = 0;
    float smoothFreq2 = 0;
    float smoothFreq3 = 0;
    
    /// Gain Attributes
    float smoothGain0 = 0;
    float smoothGain1 = 0;
    float smoothGain2 = 0;
    float smoothGain3 = 0;
    
    /// Phase Attributes
    float phase0 = 0;
    float phase1 = 0;
    float phase2 = 0;
    float phase3 = 0;
    
    /// Our ADSR
    ADSR adsr;
    
    /// APVTS parameters
    std::atomic<float>* freq1Parameter = nullptr;
    std::atomic<float>* freq2Parameter = nullptr;
    std::atomic<float>* freq3Parameter = nullptr;
    
    std::atomic<float>* gain0Parameter = nullptr;
    std::atomic<float>* gain1Parameter = nullptr;
    std::atomic<float>* gain2Parameter = nullptr;
    std::atomic<float>* gain3Parameter = nullptr;
    
    std::atomic<float>* attackParameter  = nullptr;
    std::atomic<float>* decayParameter   = nullptr;
    std::atomic<float>* sustainParameter = nullptr;
    std::atomic<float>* releaseParameter = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthVoice)
};
