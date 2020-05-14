#pragma once
#include <JuceHeader.h>
#include "ID_APVTS.h"

class ReverbComponent    : public Component
{
public:
    ReverbComponent(AudioProcessorValueTreeState& t) :
        drweAttach (t, ID_REVDRWE, drweSlider),
        sizeAttach (t, ID_REVSIZE, sizeSlider)
    {
        sliderSetup (drweSlider);
        sliderSetup (sizeSlider);
        labelSetup  (drweLabel, "Dry/Wet");
        labelSetup  (sizeLabel, "Room Size");
    }

    ~ReverbComponent()
    {
    }

    void paint (Graphics& g) override
    {
        drweLabel.setBounds  (0,                 15, getWidth() / 2.0f, 15);
        sizeLabel.setBounds  (getWidth() / 2.0f, 15, getWidth() / 2.0f, 15);
        
        drweSlider.setBounds (0,                 45, getWidth() / 2.0f, getHeight() - 55);
        sizeSlider.setBounds (getWidth() / 2.0f, 45, getWidth() / 2.0f, getHeight() - 55);
    }

    void resized() override
    {
    }

private:
    /// Default setup for labels
    void labelSetup (Label& l, String s)
    {
        addAndMakeVisible (l);
        l.setText (s, dontSendNotification);
        l.setJustificationType (Justification::centred);
    }
    
    /// Default setup for the freq sliders as rotary knobs
    void sliderSetup (Slider& s)
    {
        addAndMakeVisible(s);
        s.setSliderStyle  (Slider::RotaryVerticalDrag);
        s.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    }

    /// Labels
    Label  drweLabel, sizeLabel;
    
    /// Sliders
    Slider drweSlider, sizeSlider;
    
    /// Attachments
    AudioProcessorValueTreeState::SliderAttachment drweAttach, sizeAttach;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbComponent)
};
