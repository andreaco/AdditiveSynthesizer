#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SimpleLookAndFeel.h"

AdditiveSynthesisAudioProcessorEditor::AdditiveSynthesisAudioProcessorEditor (AdditiveSynthesisAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (800, 330);
    /// Set custom L&F
    LookAndFeel::setDefaultLookAndFeel (SimpleLookAndFeel::getInstance());
    
    /// Custom Component Setup
    masterComponent.reset (new MasterComponent (p.apvts));
    addAndMakeVisible (*masterComponent);
    
    oscillatorsComponent.reset (new OscillatorsComponent (p.apvts));
    addAndMakeVisible (*oscillatorsComponent);
    
    adsrComponent.reset (new ADSRComponent (p.apvts));
    addAndMakeVisible (*adsrComponent);
    
    reverbComponent.reset (new ReverbComponent (p.apvts));
    addAndMakeVisible (*reverbComponent);
    
    /// Title setup
    titleLabel.setFont (SimpleLookAndFeel::getInstance()->getCustomFontLight().withHeight(24));
    titleLabel.setText ("ADDITIVE SYNTHESIZER", dontSendNotification);
    titleLabel.setJustificationType (Justification::centred);
    addAndMakeVisible (titleLabel);
}

AdditiveSynthesisAudioProcessorEditor::~AdditiveSynthesisAudioProcessorEditor()
{
    // This is necessary to deallocate correctly the CustomLookAndFeel
    setLookAndFeel (nullptr);
}

void AdditiveSynthesisAudioProcessorEditor::paint (Graphics& g)
{
    /// Title position
    titleLabel.setBounds (0, 0, getWidth(), 50);
    
    /// Background Image setup
    Image bg = ImageCache::getFromMemory (BinaryData::modernbg_png, BinaryData::modernbg_pngSize);
    g.drawImageAt (bg, 0, 0);
    
    /// Top left and Bottom Right margin
    juce::Point<double> tl (20, 40);
    juce::Point<double> br (getWidth() - 20, getHeight() - 20);

    /// Master box
    auto box1w = 0.2 * (getWidth() * 5.0 / 8.0 - tl.x);
    auto box1h = br.y - tl.y - 10;
    
    /// Oscillators box
    auto box2w = 0.8 * (getWidth() * 5.0 / 8.0 - tl.x);
    auto box2h = box1h;
    
    /// ADSR & Reverb box
    auto box3w = (getWidth() * 3.0 / 8.0 - tl.x);
    auto box3h = box1h / 2.0f;
    
    /// Set components position
    masterComponent->      setBounds (tl.x,               tl.y + 10,                box1w, box1h);
    oscillatorsComponent-> setBounds (tl.x + box1w,       tl.y + 10,                box2w, box2h);
    adsrComponent->        setBounds (tl.x + box1w+box2w, tl.y + 10,                box3w, box3h);
    reverbComponent->      setBounds (tl.x + box1w+box2w, tl.y + 10 + box1h / 2.0f, box3w, box3h);
    
    /// Separator Lines draw
    g.setColour (juce::Colour (150, 150, 150).withAlpha (0.2f));
    for (int i = 1; i < 6; ++i)
        g.drawLine (tl.x + box1w * i, getHeight() * 0.3, tl.x + box1w * i, getHeight() * 0.9);
    g.drawLine (tl.x + box1w + box2w + 40, tl.y + 10 + box1h / 2.0f, br.x - 40, tl.y + 10 + box1h / 2.0f);
}

void AdditiveSynthesisAudioProcessorEditor::resized() {}
