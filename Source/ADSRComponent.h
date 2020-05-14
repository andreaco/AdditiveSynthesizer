#pragma once

#include <JuceHeader.h>

class ADSRComponent : public Component
{
public:
    ADSRComponent(AudioProcessorValueTreeState& t) :
        atkAttach (t, ID_ATK, atkSlider),
        decAttach (t, ID_DEC, decSlider),
        susAttach (t, ID_SUS, susSlider),
        relAttach (t, ID_REL, relSlider)
    {
        /// Labels setup
        labelSetup  (atkLabel, "A");
        labelSetup  (decLabel, "D");
        labelSetup  (susLabel, "S");
        labelSetup  (relLabel, "R");
        
        /// Sliders setup
        sliderSetup (atkSlider);
        sliderSetup (decSlider);
        sliderSetup (susSlider);
        sliderSetup (relSlider);
    }

    ~ADSRComponent() {}

    void paint (Graphics& g) override
    {
        auto w = getWidth();
        auto h = getHeight();
        
        atkLabel .setBounds (w * 0.0f / 4.0f, 0, w / 4.0f, 15);
        decLabel .setBounds (w * 1.0f / 4.0f, 0, w / 4.0f, 15);
        susLabel .setBounds (w * 2.0f / 4.0f, 0, w / 4.0f, 15);
        relLabel .setBounds (w * 3.0f / 4.0f, 0, w / 4.0f, 15);
        
        atkSlider.setBounds (w * 0.0f / 4.0f, 15, w / 4.0f, h - 15);
        decSlider.setBounds (w * 1.0f / 4.0f, 15, w / 4.0f, h - 15);
        susSlider.setBounds (w * 2.0f / 4.0f, 15, w / 4.0f, h - 15);
        relSlider.setBounds (w * 3.0f / 4.0f, 15, w / 4.0f, h - 15);
    }

    void resized() override {}

private:
    ///Default setup for the gain sliders as vertical linear sliders
    void sliderSetup (Slider& s)
    {
        addAndMakeVisible(s);
        auto boxDim = 50;
        s.setSliderStyle  (Slider::SliderStyle::LinearVertical);
        s.setTextBoxStyle (Slider::TextBoxBelow, false, boxDim, boxDim*0.3);
        s.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    }

    ///Default setup for labels
    void labelSetup (Label& l, String s)
    {
        addAndMakeVisible (l);
        l.setText (s, dontSendNotification);
        l.setJustificationType (Justification::centred);
    }
    
private:
    /// Sliders
    Slider atkSlider, decSlider, susSlider, relSlider;
    
    /// Labels
    Label atkLabel, decLabel, susLabel, relLabel;
    
    /// Attachments
    AudioProcessorValueTreeState::SliderAttachment atkAttach, decAttach, susAttach, relAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
