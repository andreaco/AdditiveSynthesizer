#pragma once
#include <JuceHeader.h>

class OscLookAndFeel : public LookAndFeel_V4, public DeletedAtShutdown
{
    /// Colour palette used by this L&F
    Colour sliderBack  { 75,  75,  75};
    Colour sliderFront {184, 184, 184};
    Colour red         {186,  51,  55};
    Colour green       {163, 210,  45};
    Colour white       {205, 205, 205};
    
public:
    JUCE_DECLARE_SINGLETON (OscLookAndFeel, false)
    
    OscLookAndFeel()
    {
        /// Dark Color Scheme
        setColourScheme ({
            0xff000000, //windowBackground
            0xff000000, //widgetBackground
            0xb4141414, //menuBackground
            0xb4C8C8C8, //outline
            0xffffffff, //defaultText
            0xb43c3c3c, //defaultFill
            0xffffffff, //highlightedText
            0xb43c3c3c, //highlightedFill
            0xffffffff, //menuText
        });
        
        /// Default Font
        setDefaultSansSerifTypeface (getCustomFont().withStyle (Font::bold).getTypeface());
        
        /// Label, colours
        setColour (juce::Label::textColourId, white);
    }
    
    ~OscLookAndFeel ()
    {
        ///Needed to delete singleton instance
        clearSingletonInstance();
    }
    
    /// Standard Font
    static const Font& getCustomFont()
    {
        static Font customFont (Font (Typeface::createSystemTypefaceFor (BinaryData::BakersfieldBold_ttf, BinaryData::BakersfieldBold_ttfSize)));
        return customFont;
    }
    
    /// Light Version
    static const Font& getCustomFontLight()
    {
        static Font customFont (Font (Typeface::createSystemTypefaceFor (BinaryData::BakersfieldLight_ttf,                                                                         BinaryData::BakersfieldLight_ttfSize)));
        return customFont;
    }
    
    
    /// Linear Slider Draw Function
    void drawLinearSlider (Graphics&        g,
                           int              x,
                           int              y,
                           int          width,
                           int         height,
                           float    sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const Slider::SliderStyle style,
                           Slider& slider) override
    {
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
        
        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? height * 0.25f : width * 0.25f);

        Point<float> startPoint (slider.isHorizontal() ? x : x + width * 0.5f,
                                 slider.isHorizontal() ? y + height * 0.5f : height + y);

        Point<float> endPoint (slider.isHorizontal() ? width + x : startPoint.x,
                               slider.isHorizontal() ? startPoint.y : y);
        
        
        float b =  (slider.getValue() - slider.getMinimum()) / slider.getMaximum();
        
        // Clipping
        b = b > 1 ? 1 : b;
        b = b < 0.5 ? 0 : (b - 0.5) * 2; //"relu" from 0.5 to 1
        
        Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (sliderBack.withAlpha (isMouseOver ? 1.0f : 0.7f));
        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> minPoint, maxPoint;
        
        auto kx = slider.isHorizontal() ? sliderPos : (x + width * 0.5f);
        auto ky = slider.isHorizontal() ? (y + height * 0.5f) : sliderPos;

        minPoint = startPoint;
        maxPoint = { kx, ky };
    
        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (maxPoint);
        g.setColour (red.withSaturation (b * 0.8)); //just a bit less intense red
        
        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
    }
    
    
    void drawRotarySlider (Graphics& g,
                           int                        x,
                           int                        y,
                           int                    width,
                           int                   height,
                           float              sliderPos,
                           const float rotaryStartAngle,
                           const float rotaryEndAngle,
                           Slider& slider) override
    {
        const float radius = jmin (width / 2, height / 2) - 2.0f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
    
        // Distance from 1 in range 0.5 to 2.0
        float b =  (slider.getValue() - 1.0f);
        b = b > 0 ? b : 2 * abs (b);
        // Clipping
        b = b > 1 ? 1 : b;
        b = b < 0 ? 0 : b;
        
        // Text Label
        String valstr = String((round(slider.getValue() * 100.0f)) / 100.0f);
        
        g.setColour (green.withSaturation (b));
        g.setFont (11.0f);
        g.drawText (valstr, (int) centreX - 12, (int) centreY - 8, 24, 16, Justification::centred);
    
        const float thickness = 0.9f;
        
        //Back
        g.setColour (sliderBack.withAlpha(isMouseOver ? 1.0f : 0.7f));
        Path outlineArc;
        outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
        outlineArc.closeSubPath();

        g.fillPath (outlineArc.createPathWithRoundedCorners (2.0f));
        
        // Front
        g.setColour (green.withSaturation (b));
        
        Path filledArc;
        filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
        
        g.fillPath (filledArc.createPathWithRoundedCorners (2.0f));
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscLookAndFeel)
};
