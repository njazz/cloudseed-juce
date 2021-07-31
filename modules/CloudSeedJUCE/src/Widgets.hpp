///> @file all ui elements

#pragma once

#include <JuceHeader.h>

#include "Layout.hpp"

///> pointer used in layouts
using WidgetPtr = juce::Component*;

///> common class for layout views
class LayoutViewCommon : public juce::Component {
protected:
    std::vector<WidgetPtr> _subviews = {};
    LayoutData _layoutData;// = std::make_shared<LayoutData>();

public:
    void setLayout(const LayoutData& d);

    void addSubview(WidgetPtr);
    void removeAllSubviews();
};

///> view that puts components in a row with specified layout
class HorizontalLayoutView : public LayoutViewCommon {
public:
    void resized() override;

    static std::shared_ptr<HorizontalLayoutView> Create() { return std::make_shared<HorizontalLayoutView>(); }
};

using HorizontalLayoutViewPtr = std::shared_ptr<HorizontalLayoutView>;

///> view that puts components in a column with specified layout
class VerticalLayoutView : public LayoutViewCommon {
public:
    void resized() override;

    static std::shared_ptr<VerticalLayoutView> Create() { return std::make_shared<VerticalLayoutView>(); }
};

using VerticalLayoutViewPtr = std::shared_ptr<VerticalLayoutView>;

// ---

///> gets bundled font
static inline const juce::Font GetCloudSeedFont(){
    return Font (Typeface::createSystemTypefaceFor (BinaryData::OpenSansRegular_ttf,
                                                  BinaryData::OpenSansRegular_ttfSize));
}


///> gets bundled font with specific size
static inline const juce::Font GetCloudSeedFont(const float& size){
    auto f = GetCloudSeedFont();
    f.setHeight(size);
    return f;
}

///> color & font settings
struct ColorTheme {
    juce::Colour backgroundColor = juce::Colour::fromFloatRGBA(.9,.9,.9,1);
    juce::Colour groupBackgroundColor = juce::Colour::fromFloatRGBA(.87,.87,.87,1);
    juce::Colour buttonBackgroundColor = juce::Colour::fromFloatRGBA(.67,.67,.67,1);;
    juce::Colour groupColor = juce::Colour::fromFloatRGBA(.7,.7,.7,1);
    juce::Colour knobColor = juce::Colour::fromFloatRGBA(.0,.5,1,1);;
    juce::Colour knobValueBackground = juce::Colour::fromFloatRGBA(.7,.7,.7,.5);;
    juce::Colour defaultTextColor = juce::Colour::fromFloatRGBA(.2,.2,.2,1);
    juce::Colour buttonOffBackground = juce::Colour::fromFloatRGBA(.5,.5,.5,1);
    juce::Colour buttonOnBackground = juce::Colour::fromFloatRGBA(.0,.5,1,1);
    juce::Colour buttonTextOn;
    juce::Colour buttonTextOff;

    juce::Font font = GetCloudSeedFont(40);//Font("Helvetica",48.0,juce::Font::FontStyleFlags::plain);
    juce::Font fontSmall = GetCloudSeedFont(13);//Font("Helvetica",11.0,juce::Font::FontStyleFlags::plain);
};

///> currently the only theme
extern const ColorTheme _theColorTheme;
static inline const ColorTheme& CurrentColorTheme(){ return _theColorTheme; }

///> all UI elements
namespace Widgets{

using FnAction = std::function<void(void)>;
using FnDisplayText = std::function<std::string(const float&)>;
using FnGetDefaultValue = std::function<float(void)>;

///> Button & value. Supports drag
class ButtonSlider : public juce::Component{
public:
    float value = 0;
    int min = 0;
    int max = 1;

    std::string name = "";
    int parameterIndex = -1;

    FnAction fnAction = [](){};
    FnDisplayText fnDisplay = [](const bool& v)->std::string{
            return v?"On": "Off";
    };

    void paint(juce::Graphics &g) override;

    void mouseDown(const juce::MouseEvent &) override;
    void mouseDrag(const juce::MouseEvent &event) override;
};

///> basic knob
class Knob : public juce::Component{
    juce::Point<int> _lastPosition;
public:
    Knob() : juce::Component(){}

    float value =0.;    // 0..1
    std::string name = "-";
    int parameterIndex = -1;

    FnAction fnAction = [](){};
    FnDisplayText fnDisplay = [](const float&)->std::string{ return "0.0"; };
    FnGetDefaultValue fnGetDefault = [](){ return 0.0f; };

    void paint(juce::Graphics &g) override;

    void mouseDown(const juce::MouseEvent &) override;
    void mouseDrag(const juce::MouseEvent &e) override;
    void mouseDoubleClick(const juce::MouseEvent &) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
};

// ---

///> group of UI controls, base class
class BaseGroup : public VerticalLayoutView {
public:
    std::string name = "";

    void paint(juce::Graphics &g) override;
};

///> group of UI controls
class Group : public BaseGroup {
public:
    Group();

    std::array<HorizontalLayoutView,3> rows;
};

///> group of random seed UI controls
class RandomSeedGroup : public BaseGroup {
public:
    RandomSeedGroup();

    HorizontalLayoutView labels;
    HorizontalLayoutView buttons;
};

///> group for parameter info view
class InfoGroup : public BaseGroup {
public:
    std::string value = "";

    void paint(juce::Graphics &g) override;
};

///> group for program name
class ProgramGroup : public InfoGroup{
public:
    std::function<void(void)> fnInc = [](){};
    std::function<void(void)> fnDec = [](){};

    void paint(juce::Graphics &g) override;

    void mouseDown(const juce::MouseEvent &event) override;

};

// ---

///> text input field with int value
class TextIntInput : public juce::Component {
public:
    FnAction fnAction = [](){};

    void paint(juce::Graphics &g) override;
};

///> label for preset name
class PresetNameLabel :public juce::Component {
public:
    std::string name = "";

    void paint(juce::Graphics &g) override;
};

};
