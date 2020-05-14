#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MasterComponent.h"
#include "OscillatorsComponent.h"
#include "ADSRComponent.h"
#include "ReverbComponent.h"

class AdditiveSynthesisAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AdditiveSynthesisAudioProcessorEditor (AdditiveSynthesisAudioProcessor&);
    ~AdditiveSynthesisAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    AdditiveSynthesisAudioProcessor& processor;
    
    /// Title
    Label titleLabel;
    
    /// Custom Components
    std::unique_ptr<MasterComponent> masterComponent;
    std::unique_ptr<OscillatorsComponent> oscillatorsComponent;
    std::unique_ptr<ADSRComponent> adsrComponent;
    std::unique_ptr<ReverbComponent> reverbComponent;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdditiveSynthesisAudioProcessorEditor)
};
