#pragma once

#include <JuceHeader.h>

#include "ColorTheme.hpp"

#include "Layout.hpp"

#include "DataModel.hpp"
#include "EngineCore.hpp"

class BaseComponent : public juce::Component//,
//                      public IDataView
{
protected:
    bool _midiLearnMode = false;
public:
    BaseComponent();

    void setMidiLearnMode(const bool& b){
        _midiLearnMode = b;
        repaint();
    }

//    void setNeedsUpdate() override { repaint(); }

};

using WidgetPtr = std::shared_ptr<BaseComponent>;

// ----------
struct LabelData {
    juce::Colour color = SoundfruuzeColor::white;
    Justification justification = Justification::centred;
    std::string text;
};

class BasicLabel : public LabelData, public BaseComponent {
public:
    BasicLabel(const std::string& s_ = "Label");

    void paint(Graphics& g) override;

    static std::shared_ptr<BasicLabel> Create(const std::string& s_ = "Label") { return std::make_shared<BasicLabel>(s_); }
};
using BasicLabelPtr = std::shared_ptr<BasicLabel>;

// ---

using FnWidgetAction = std::function<void(void)>;

struct ButtonCommon {
    juce::Colour colorOff = SoundfruuzeColor::gray;
    juce::Colour colorOn = SoundfruuzeColor::lightBlue;
    juce::Colour textColorOff = SoundfruuzeColor::white;
    juce::Colour textColorOn = SoundfruuzeColor::white;
    std::string text;
    juce::Image image;
    float cornerRadius = 5;

    bool isMouseDown = false;

    bool alwaysUseImage = false;
    float widthForImage = 0; ///< if non-zero and less than value, the image is used instead of text

    FnWidgetAction fnAction = []() {};
};

class BasicButton : public ButtonCommon, public BaseComponent {
public:
    void paint(Graphics& g) override;

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    static std::shared_ptr<BasicButton> Create() { return std::make_shared<BasicButton>(); }
};

using BasicButtonPtr = std::shared_ptr<BasicButton>;

struct ToggleCommon {
    bool value = false;
};

struct TriCommon {
    TriState value = TriState::Off;
};

// ---

class BasicToggle : public ButtonCommon, public ToggleCommon, public BaseComponent {
public:
    void paint(Graphics& g) override;

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    static std::shared_ptr<BasicToggle> Create() { return std::make_shared<BasicToggle>(); }
};

using BasicTogglePtr = std::shared_ptr<BasicToggle>;

// ------------------------------

struct RoundCommon : public ButtonCommon {
    juce::Colour borderColor = SoundfruuzeColor::white;
    bool noFillOffState = false;    ///< keep toggle without fill if true
};

class RoundButton : public RoundCommon, public BaseComponent {
public:
    void paint(Graphics& g) override;

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    static std::shared_ptr<RoundButton> Create() { return std::make_shared<RoundButton>(); }
};

using RoundButtonPtr = std::shared_ptr<RoundButton>;

// ---

class RoundToggle : public RoundCommon, public ToggleCommon, public BaseComponent {
public:
    void paint(Graphics& g) override;

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    static std::shared_ptr<RoundToggle> Create() { return std::make_shared<RoundToggle>(); }
};

using RoundTogglePtr = std::shared_ptr<RoundToggle>;

// ---
class RoundToggleTri : public RoundCommon, public TriCommon, public BaseComponent {
public:

    void paint(Graphics& g) override;

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    static std::shared_ptr<RoundToggleTri> Create() { return std::make_shared<RoundToggleTri>(); }
};

using RoundToggleTriPtr = std::shared_ptr<RoundToggleTri>;

// ------------------------------

class TriangleToggle : public BaseComponent, public ButtonCommon, public ToggleCommon {
public:
    void paint(Graphics& g) override;

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    static std::shared_ptr<TriangleToggle> Create() { return std::make_shared<TriangleToggle>(); }
};

using TriangleTogglePtr = std::shared_ptr<TriangleToggle>;

// ------------------------------

class ISliderValueDelegate {
    ~ISliderValueDelegate() { }

public:
    virtual void onSliderValueChanged() = 0;
};

///> @deprecated fix in value
class SliderValue {

    using ConverterFn = std::function<float(const float&)>;

    ConverterFn _fnTo = [](const float& f) { return f; };
    ConverterFn _fnFrom = [](const float& f) { return f; };

    float _min = 0;
    float _max = 1;

    float _value = 0;

public:
    ISliderValueDelegate* delegate = nullptr;

    void setMinimum(const float&);
    void setMaximum(const float&);

    void setNormalizedValue(const float&);
    void setValue(const float&);

    float getNormalizedValue();
    float getValue();

    float getRangeLength();

    void setConverterToNormalizedValue(const ConverterFn&);
    void setConverterFromNormalizedValue(const ConverterFn&);
};

// ---

struct SliderCommon {
protected:
    EngineCore::FloatValueClass valueClass;
public:
    void setValueClass(const EngineCore::FloatValueClass & vc_){
        valueClass = vc_;
        value = vc_.initial;
    }
//    std::string name;
//    std::string units;
//    SliderValue value;
    float cornerRadius = 5;
    float value;

    bool isMouseDown = false;

    FnWidgetAction fnAction = []() {};
};

class HorizontalSlider : public SliderCommon, public BaseComponent {
    float _toValue(const float& x);
public:
//    HorizontalSlider();
    void paint(Graphics& g);

    void mouseDown(const MouseEvent &event);
    void mouseDrag(const MouseEvent &event);
    void mouseUp(const MouseEvent &event);

    void mouseDoubleClick(const MouseEvent &event);

    static std::shared_ptr<HorizontalSlider> Create() { return std::make_shared<HorizontalSlider>(); }
};
using HorizontalSliderPtr = std::shared_ptr<HorizontalSlider>;

// ---
class VerticalSlider : public SliderCommon, public BaseComponent {
public:
    void paint(Graphics& g);

    static std::shared_ptr<VerticalSlider> Create() { return std::make_shared<VerticalSlider>(); }
};

using VerticalSliderPtr = std::shared_ptr<VerticalSlider>;

// ------------------------------

struct TabsCommon : public ButtonCommon {
    std::vector<std::string> options;
    size_t value = 0;

    //    FnWidgetAction fnAction = []() {};
};

class HorizontalTabs : public TabsCommon, public BaseComponent {
public:
    void paint(Graphics& g);

    void mouseDown(const MouseEvent& event);

    static std::shared_ptr<HorizontalTabs> Create() { return std::make_shared<HorizontalTabs>(); }
};

using HorizontalTabsPtr = std::shared_ptr<HorizontalTabs>;

class VerticalTabs : public TabsCommon, public BaseComponent {
public:
    void paint(Graphics& g);

    void mouseDown(const MouseEvent& event);

    static std::shared_ptr<VerticalTabs> Create() { return std::make_shared<VerticalTabs>(); }
};

using VerticalTabsPtr = std::shared_ptr<VerticalTabs>;

// =============================================

class LayoutViewCommon : public BaseComponent {
protected:
    std::vector<WidgetPtr> _subviews = {};
    std::shared_ptr<LayoutData> _layoutData = std::make_shared<LayoutData>();

public:
    void setLayout(const LayoutData& d);

    void addSubview(WidgetPtr);
    void removeAllSubviews();
};

class HorizontalLayoutView : public LayoutViewCommon {
public:
    void resized() override;

    static std::shared_ptr<HorizontalLayoutView> Create() { return std::make_shared<HorizontalLayoutView>(); }
};

using HorizontalLayoutViewPtr = std::shared_ptr<HorizontalLayoutView>;

class VerticalLayoutView : public LayoutViewCommon {
public:
    void resized() override;

    static std::shared_ptr<VerticalLayoutView> Create() { return std::make_shared<VerticalLayoutView>(); }
};

using VerticalLayoutViewPtr = std::shared_ptr<VerticalLayoutView>;

// =============================================

class PagesView : public BaseComponent {
    std::vector<WidgetPtr> _pages;
    size_t _currentPage = (size_t)-1;

public:
    void addPage(WidgetPtr);
    void showPage(const size_t&);

    void resized() override;

    static std::shared_ptr<PagesView> Create() { return std::make_shared<PagesView>(); }
};

using PagesViewPtr = std::shared_ptr<PagesView>;

// =============================================
// special tab controls:

struct VerticalMultiTabsViewData : public ButtonCommon {
    juce::Colour colorOff = SoundfruuzeColor::gray;
    juce::Colour colorOn = SoundfruuzeColor::lightBlue;
    float cornerRadius = 5;

    std::vector<bool> values;
    // use text for button names
};

// single name
class VerticalMultiTabsView : public VerticalMultiTabsViewData, public BaseComponent {
public:
    void paint(Graphics& g);

    void mouseDown(const MouseEvent& event);

    static std::shared_ptr<VerticalMultiTabsView> Create() { return std::make_shared<VerticalMultiTabsView>(); }
};

using VerticalMultiTabsViewPtr = std::shared_ptr<VerticalMultiTabsView>;

// ---
struct StringWithFlag {
    std::string value = "";
    bool flag = false;

    StringWithFlag(const std::string& v_, const bool& f_)
        : value(v_)
        , flag(f_)
    {
    }
};

struct OptionalVerticalTabsViewData : public ButtonCommon {
    juce::Colour colorOff = SoundfruuzeColor::gray;
    juce::Colour colorOn = SoundfruuzeColor::lightBlue;
    float cornerRadius = 5;

    std::vector<StringWithFlag> options;

    size_t value = 0;
};

class OptionalVerticalTabsView : public OptionalVerticalTabsViewData, public BaseComponent {
public:
    void paint(Graphics& g);

    void mouseDown(const MouseEvent& event);

    static std::shared_ptr<OptionalVerticalTabsView> Create() { return std::make_shared<OptionalVerticalTabsView>(); }
};

using OptionalVerticalTabsViewPtr = std::shared_ptr<OptionalVerticalTabsView>;

// ---
struct HideableVerticalTabsViewData : public ButtonCommon {
    juce::Colour colorOff = SoundfruuzeColor::gray;
    juce::Colour colorOn = SoundfruuzeColor::lightBlue;
    float cornerRadius = 5;

    std::vector<bool> hidden;
    size_t value = 0;
    // use text for button names
};

// single name
class HideableVerticalTabsView : public HideableVerticalTabsViewData, public BaseComponent {
public:
    void paint(Graphics& g);

    void mouseDown(const MouseEvent& event);

    static std::shared_ptr<HideableVerticalTabsView> Create() { return std::make_shared<HideableVerticalTabsView>(); }
};

using HideableVerticalTabsViewPtr = std::shared_ptr<HideableVerticalTabsView>;

// ------------------------------------------------------

namespace ToolbarWidget {

RoundButtonPtr createCloseAllButton();

BasicTogglePtr createPauseButton();
BasicTogglePtr createForcePauseButton();

WidgetPtr createTapButton();

BasicTogglePtr createThruButton();

HorizontalSliderPtr createInputLevelSlider();
HorizontalSliderPtr createOutputLevelSlider();

BasicTogglePtr createLimiterButton();
BasicTogglePtr createSoftclipButton();

BasicTogglePtr createZoomButton();
BasicTogglePtr createSnapButton();

HorizontalSliderPtr createBPMSlider();
HorizontalTabsPtr createBeatDivisionTabs();

HorizontalTabsPtr createToolbarTabs();
// ---

RoundTogglePtr createReverseRoundButton();
RoundButtonPtr createRetriggerRoundButton();
RoundTogglePtr createPauseRoundButton();

RoundToggleTriPtr createLanePauseRoundButton();

RoundToggleTriPtr createLooperLockSizeRoundButton();

///> @deprecated
WidgetPtr createTransparentRoundButton();

// new side buttons:
RoundTogglePtr createRecordFromUpperRoundButton();
RoundTogglePtr createDisablePauseRoundButton();
RoundTogglePtr createMaximizeRoundButton();
RoundTogglePtr createNewLaneBelowButton();
RoundTogglePtr createMuteRoundButton();

WidgetPtr createHoldButton();

BasicButtonPtr createHelpButton();

RoundButtonPtr createCtrlDefaultButton();
RoundButtonPtr createCtrlAllButton();
RoundButtonPtr createCtrlNoneButton();

RoundButtonPtr createCtrlResetButton();

RoundButtonPtr createCtrlNoneButton();

RoundButtonPtr createAddLaneButton();
RoundButtonPtr createRemoveLaneButton();

BasicTogglePtr createSnapshotSelectorButton();
BasicTogglePtr createRemoteSetupButton();
BasicTogglePtr createMIDILearnButton();

BasicButtonPtr createSnapshotResetButton();
BasicTogglePtr createSnapshotNumberButton(const size_t idx);
BasicTogglePtr createSnapshotWriteButton();

};
