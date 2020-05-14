#pragma once

#include <JuceHeader.h>
#include "ID_APVTS.h"

class MasterComponent : public Component
{
public:
    MasterComponent(AudioProcessorValueTreeState& t) :
        masterAttach (t, ID_MASTER, masterSlider)
    {
        /// Sliders setup
        addAndMakeVisible(masterSlider);
        masterSlider.setSliderStyle (Slider::SliderStyle::LinearVertical);
        masterSlider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        
        /// Labels setup
        addAndMakeVisible (masterLabel);
        masterLabel.setText ("Master", dontSendNotification);
        masterLabel.setJustificationType (Justification::centred);
    }

    ~MasterComponent() {}

    void paint (Graphics& g) override
    {
        /// Position and margin variables
        auto lh = 15;
        auto margin = 10;
        auto sY = lh + margin;
        
        /// Label and slider setup
        masterLabel .setBounds (0,  0, getWidth(), lh);
        masterSlider.setBounds (0, sY, getWidth(), getHeight() - sY);
    }

    void resized() override {}

private:
    /// Slider and Label
    Slider masterSlider;
    Label masterLabel;
    
    /// Attachment
    AudioProcessorValueTreeState::SliderAttachment masterAttach;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterComponent)
};
