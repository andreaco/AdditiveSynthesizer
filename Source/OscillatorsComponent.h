#pragma once

#include <JuceHeader.h>
#include "ID_APVTS.h"
#include "OscLookAndFeel.h"

class OscillatorsComponent : public Component
{
public:
    
    OscillatorsComponent(AudioProcessorValueTreeState& t) :
        gain0Attach  (t,  ID_GAIN0, gain0Slider),
        gain1Attach  (t,  ID_GAIN1, gain1Slider),
        gain2Attach  (t,  ID_GAIN2, gain2Slider),
        gain3Attach  (t,  ID_GAIN3, gain3Slider),
        freq1Attach  (t,  ID_FREQ1, freq1Slider),
        freq2Attach  (t,  ID_FREQ2, freq2Slider),
        freq3Attach  (t,  ID_FREQ3, freq3Slider)
    {
        
        labelSetup (gain0Label,  String ("Gain 0"));
        labelSetup (gain1Label,  String ("Gain 1"));
        labelSetup (gain2Label,  String ("Gain 2"));
        labelSetup (gain3Label,  String ("Gain 3"));
        labelSetup (freq1Label,  String ("Freq 1"));
        labelSetup (freq2Label,  String ("Freq 2"));
        labelSetup (freq3Label,  String ("Freq 3"));
        
        gainSliderSetup (gain0Slider);
        gainSliderSetup (gain1Slider);
        gainSliderSetup (gain2Slider);
        gainSliderSetup (gain3Slider);
        freqSliderSetup (freq1Slider);
        freqSliderSetup (freq2Slider);
        freqSliderSetup (freq3Slider);
    }

    ~OscillatorsComponent() {}

    void paint (Graphics& g) override
    {
        auto w = getWidth();
        auto h = getHeight();
        
        auto margin = 10;
        
        /// Labels Freq
        auto lw = w * (1/4.0f);
        auto lh = 15;
        gain0Label.setBounds (w * (0 / 4.0f), 0, lw, lh);
        freq1Label.setBounds (w * (1 / 4.0f), 0, lw, lh);
        freq2Label.setBounds (w * (2 / 4.0f), 0, lw, lh);
        freq3Label.setBounds (w * (3 / 4.0f), 0, lw, lh);
        
        /// Frequency sliders on the first row
        auto scale = 0.5f;
        auto sw =  scale * w * (1 / 4.0f);
        auto off = (1 - scale) * w * (1 / 4.0f) * 0.5;
        auto sYf = lh + margin;
        freq1Slider.setBounds (off + w * (1/4.0f), sYf, sw, sw);
        freq2Slider.setBounds (off + w * (2/4.0f), sYf, sw, sw);
        freq3Slider.setBounds (off + w * (3/4.0f), sYf, sw, sw);
        
        /// Labels Gain
        auto slg = sYf + sw + 2 * margin;

        gain1Label.setBounds (w * (1 / 4.0f), slg, lw, lh);
        gain2Label.setBounds (w * (2 / 4.0f), slg, lw, lh);
        gain3Label.setBounds (w * (3 / 4.0f), slg, lw, lh);
        
        auto sYg = slg + lh +margin;
        auto sh = h - sYg;
        sw = w * (1 / 4.0f);
        
        /// Gain sliders on the second row
        gain0Slider.setBounds (w * (0 / 4.0f), sYf, sw, h - sYf);
        gain1Slider.setBounds (w * (1 / 4.0f), sYg, sw, sh);
        gain2Slider.setBounds (w * (2 / 4.0f), sYg, sw, sh);
        gain3Slider.setBounds (w * (3 / 4.0f), sYg, sw, sh);
    }

    void resized() override {}

private:
    /**
       Default setup for the freq sliders as rotary knobs
    */
    void freqSliderSetup (Slider& s)
    {
        addAndMakeVisible (s);
        s.setSliderStyle (Slider::RotaryVerticalDrag);
        s.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        s.setLookAndFeel (OscLookAndFeel::getInstance());
    }

    /**
       Default setup for the gain sliders as vertical linear sliders
    */
    void gainSliderSetup (Slider& s)
    {
        addAndMakeVisible (s);
        auto boxDim = 50;
        s.setSliderStyle (Slider::SliderStyle::LinearVertical);
        s.setTextBoxStyle (Slider::TextBoxBelow, false, boxDim, boxDim * 0.3);
        s.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        s.setLookAndFeel (OscLookAndFeel::getInstance());
    }

    /**
       Default setup for labels
    */
    void labelSetup (Label& l, String s)
    {
        addAndMakeVisible (l);
        l.setText (s, dontSendNotification);
        l.setJustificationType (Justification::centred);
    }

    /// Sliders
    Slider gain0Slider, gain1Slider, gain2Slider, gain3Slider;
    Slider freq1Slider, freq2Slider, freq3Slider;
    
    /// Labels
    Label gain0Label, gain1Label, gain2Label, gain3Label;
    Label freq1Label, freq2Label, freq3Label;
    
    /// Attachments
    AudioProcessorValueTreeState::SliderAttachment gain0Attach, gain1Attach, gain2Attach, gain3Attach;
    AudioProcessorValueTreeState::SliderAttachment freq1Attach, freq2Attach, freq3Attach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorsComponent)
};
