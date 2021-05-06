#include "BasicWidgets.hpp"

BaseComponent::BaseComponent()
    : juce::Component()
{
    //    auto f= juce::Font("Helvetica Neue","Thin",14);
    //auto lf =
    //    getLookAndFeel().setDefaultSansSerifTypefaceName("Helvetica Neue Thin");
}

BasicLabel::BasicLabel(const std::string& s_)
{
    text = s_;
}

void BasicLabel::paint(Graphics& g)
{
    g.setColour(color);
    g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
    g.setFont(getLocalBounds().getHeight() * .5);

    g.drawText(text, getLocalBounds(), justification, false);
}
//
void BasicButton::paint(Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    g.setColour(colorOff);
    g.fillRoundedRectangle(bounds, cornerRadius);

    auto fullBounds = bounds;
    bounds.expand(-1, -1);

    if (isMouseDown) {
        g.setColour(SoundfruuzeColor::white);
        g.drawRoundedRectangle(bounds, cornerRadius, 2);
    }

    g.setColour(SoundfruuzeColor::white);
    g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
    g.setFont(bounds.getHeight() * .5);

    g.drawText(text, bounds, Justification::centred, false);
}

void BasicButton::mouseDown(const MouseEvent& event)
{
    isMouseDown = true;
    fnAction();
    repaint();
}
void BasicButton::mouseUp(const MouseEvent& event)
{
    isMouseDown = false;
    repaint();
}

// ------------------------------

void BasicToggle::paint(Graphics& g)
{
    g.setColour((value) ? colorOn : colorOff);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    g.setColour(SoundfruuzeColor::white);
    g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
    g.setFont(getLocalBounds().getHeight() * .5);
    g.drawText(text, getLocalBounds().toFloat(), Justification::centred, false);
}

void BasicToggle::mouseDown(const MouseEvent& event)
{
    isMouseDown = true;
    value = !value;
    fnAction();
    repaint();
}
void BasicToggle::mouseUp(const MouseEvent& event)
{
    isMouseDown = true;
    fnAction();
    repaint();
}

// ------------------------------

void RoundButton::paint(Graphics& g)
{
    g.setColour(colorOff);

    auto bounds = getLocalBounds().toFloat();
    auto w = (bounds.getWidth()<bounds.getHeight()) ? bounds.getWidth() : bounds.getHeight();
    bounds = Rectangle<float>(0,0,w,w);

    auto fullBounds = bounds;
    bounds.reduce(1, 1);



    if (isMouseDown) {
        g.drawEllipse(bounds, 1);
    } else {

        g.fillEllipse(fullBounds);
    }

    g.setColour(SoundfruuzeColor::white);
    g.setFont(juce::Font("Helvetica Neue", "Light", 14));

    g.setFont(bounds.getHeight() * (.4 + .1 * (text.size() == 1)));

    g.drawText(text, bounds, Justification::centred, false);
}

void RoundButton::mouseDown(const MouseEvent& event)
{
    isMouseDown = true;
    fnAction();
    repaint();
}
void RoundButton::mouseUp(const MouseEvent& event)
{
    isMouseDown = false;
    repaint();
}

// ---
void RoundToggle::paint(Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto w = (bounds.getWidth()<bounds.getHeight()) ? bounds.getWidth() : bounds.getHeight();
    bounds = Rectangle<float>(0,0,w,w);

    auto fullBounds = bounds;
    bounds.expand(-1, -1);

    if (value) {
        g.setColour(colorOn);
        g.fillEllipse(fullBounds);
    } else {
        g.setColour(SoundfruuzeColor::darkGrayTranslucent);
        g.fillEllipse(fullBounds);
        g.setColour(colorOff);
        g.drawEllipse(bounds, 1);
    }

    g.setColour(SoundfruuzeColor::white);

    g.setFont(juce::Font("Helvetica Neue", "Light", 14));
    g.setFont(getLocalBounds().getHeight() * (.4 + .1 * (text.size() == 1)));

    g.drawText(text, getLocalBounds().toFloat(), Justification::centred, false);
}

void RoundToggle::mouseDown(const MouseEvent& event)
{
    isMouseDown = true;
    value = !value;
    fnAction();
    repaint();
}
void RoundToggle::mouseUp(const MouseEvent& event)
{
    isMouseDown = false;
    repaint();
}

// ---
void RoundToggleTri::paint(Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto fullBounds = bounds;
    bounds.expand(-1, -1);

    if (value == TriState::On) {
        g.setColour(colorOn);
        g.fillEllipse(fullBounds);
    }

    if (value == TriState::HalfOn) {
        g.setColour(SoundfruuzeColor::darkGrayTranslucent);
        g.fillEllipse(fullBounds);
        g.setColour(colorOn);
        g.drawEllipse(bounds, 2);
    }

    if (value == TriState::Off) {
        g.setColour(SoundfruuzeColor::darkGrayTranslucent);
        g.fillEllipse(fullBounds);
        g.setColour(colorOff);
        g.drawEllipse(bounds, 1);
    }

    g.setColour(SoundfruuzeColor::white);

    g.setFont(juce::Font("Helvetica Neue", "Light", 14));
    g.setFont(getLocalBounds().getHeight() * (.4 + .1 * (text.size() == 1)));

    g.drawText(text, getLocalBounds().toFloat(), Justification::centred, false);
}

void RoundToggleTri::mouseDown(const MouseEvent& event)
{
    isMouseDown = true;
    //    value = !value;
    fnAction();
    repaint();
}
void RoundToggleTri::mouseUp(const MouseEvent& event)
{
    isMouseDown = false;
    repaint();
}
// ------------------------------

void TriangleToggle::paint(Graphics& g)
{
    // always this: |>

    juce::Path p;
    auto w = getLocalBounds().getWidth();
    auto h = getLocalBounds().getHeight();
    auto h2 = 0.5 * h;

    p.addTriangle(1, 1, w - 1, h2 - .5, 1, h - 1);
    g.setColour(value ? SoundfruuzeColor::lightBlue : SoundfruuzeColor::gray);
    g.fillPath(p);
    g.setColour(SoundfruuzeColor::white);
    g.strokePath(p, PathStrokeType(1));
}

void TriangleToggle::mouseDown(const MouseEvent& event)
{
    isMouseDown = true;
    value = !value;
    fnAction();
    repaint();
}
void TriangleToggle::mouseUp(const MouseEvent& event)
{
    isMouseDown = true;
    //    value = !value;
    //    fnAction();
    repaint();
}

// ------------------------------

void SliderValue::setMinimum(const float& m)
{
    _min = m;

    if (delegate)
        delegate->onSliderValueChanged();
}
void SliderValue::setMaximum(const float& m)
{
    _max = m;

    if (delegate)
        delegate->onSliderValueChanged();
}

void SliderValue::setNormalizedValue(const float& v)
{
    setValue(_fnFrom(v));
}
void SliderValue::setValue(const float& v)
{
    _value = v;

    if (delegate)
        delegate->onSliderValueChanged();
}

float SliderValue::getNormalizedValue()
{
    return _fnTo(_value);
}
float SliderValue::getValue() { return _value; }

float SliderValue::getRangeLength() { return _max - _min; }

void SliderValue::setConverterToNormalizedValue(const ConverterFn& f) { _fnTo = f; }
void SliderValue::setConverterFromNormalizedValue(const ConverterFn& f) { _fnFrom = f; }

// =============================================

void LayoutViewCommon::setLayout(const LayoutData& d)
{
    *_layoutData = d;
    resized();
}

void LayoutViewCommon::addSubview(WidgetPtr c)
{
    _subviews.push_back(c);

    addChildComponent(c.get());
    c->setVisible(true);
    resized();
}

void LayoutViewCommon::removeAllSubviews()
{
    removeAllChildren();
    _subviews.clear();
    resized();
}

//
void HorizontalLayoutView::resized()
{
    auto result = _layoutData->calculate(getLocalBounds().getWidth());
    auto h = getLocalBounds().getHeight();

    size_t idx = 0;
    float currentX = 0;
    for (auto& e : _subviews) {
        if (idx >= result.size())
            return;

        currentX += result[idx].offset;
        e->setBounds(currentX, 0, result[idx].size, h);
        currentX += result[idx].size;
        idx++;
    }
}

void VerticalLayoutView::resized()
{

    auto result = _layoutData->calculate(getLocalBounds().getHeight());
    auto w = getLocalBounds().getWidth();

    size_t idx = 0;
    float currentY = 0;
    for (auto& e : _subviews) {

        if (idx >= result.size())
            return;

        currentY += result[idx].offset;
        e->setBounds(0, currentY, w, result[idx].size);
        currentY += result[idx].size;

        idx++;
    }
}

// ---

//HorizontalSlider::HorizontalSlider(){
////    value = valueClass.initial;
////    printf("initial %f\n");
//}

void HorizontalSlider::paint(Graphics& g)
{

    //    g.fillAll(SoundfruuzeColor::darkGrayTranslucent);

    g.setColour(SoundfruuzeColor::darkGrayTranslucent);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    g.setColour(SoundfruuzeColor::gray);
    g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
    g.setFont(getLocalBounds().getHeight() * .8);
    auto b = getLocalBounds();
    b.setHeight(b.getHeight() - 1);
    g.drawText(valueClass.name, b.toFloat(), Justification::centred, false);

    // handle
    auto h = getLocalBounds().getHeight();
    auto handleWidth = h * 2;
    auto nv = valueClass.fnToNormalized(value);
    auto handleX = (getLocalBounds().getWidth() - handleWidth) * nv;
    g.setColour(SoundfruuzeColor::grayTranslucent);

    g.fillRoundedRectangle(handleX, 0, handleWidth, h, cornerRadius);

    auto valueString = valueClass.fnToString(value);
    g.setFont(getLocalBounds().getHeight() * .5);
    g.setColour(SoundfruuzeColor::white);
    g.drawText(valueString, Rectangle<float>(handleX, 0, handleWidth, h), Justification::centred, false);

    if (isMouseDown) {
        g.setColour(SoundfruuzeColor::white);
        g.drawRoundedRectangle(handleX, 0, handleWidth, h, cornerRadius, 1);
    }
}

float HorizontalSlider::_toValue(const float& x)
{
    auto h = getLocalBounds().getHeight();
    float handleWidth = h * 2;
    auto handleWidth_2 = handleWidth / 2.;

    auto fract = (x - handleWidth_2) / (getLocalBounds().getWidth() - handleWidth);
    if (fract < 0)
        fract = 0;
    if (fract > 1)
        fract = 1;

    //    printf("%f %f\n", valueClass.min, valueClass.max);

    auto ret = valueClass.fnFromNormalized(fract);

    //    printf("%f %f %f\n",x,fract,ret);
    return ret;
}

void HorizontalSlider::mouseDown(const MouseEvent& event)
{
    isMouseDown = true;
    value = _toValue(event.getMouseDownX());
    repaint();
    fnAction();
}
void HorizontalSlider::mouseDrag(const MouseEvent& event)
{
    value = _toValue(event.getMouseDownX() + event.getDistanceFromDragStartX());
    repaint();
    fnAction();
}
void HorizontalSlider::mouseUp(const MouseEvent& event)
{
    isMouseDown = false;
    repaint();
}

void HorizontalSlider::mouseDoubleClick(const MouseEvent &event){
    value = valueClass.initial;
    repaint();
    fnAction();
}

void VerticalSlider::paint(Graphics& g)
{
    g.fillAll(SoundfruuzeColor::darkGrayTranslucent);
}

// ---
void HorizontalTabs::paint(Graphics& g)
{
    g.setColour(colorOff);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    if (!options.size())
        return;

    auto bounds = getLocalBounds();
    bounds.expand(0, 0);
    auto optionWidth = bounds.getWidth() / options.size();

    size_t idx = 0;
    for (auto& e : options) {

        bounds.setWidth(optionWidth + 1);
        bounds.setX(idx * optionWidth);

        g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
        g.setFont(getLocalBounds().getHeight() * .5);

        const float& x = bounds.getX();
        const float& y = bounds.getY();

        const float w = bounds.getWidth();
        const float h = bounds.getHeight();

        bool leftCorners = (idx == 0);
        bool rightCorners = (idx == (options.size() - 1));

        Path p;
        p.startNewSubPath(x, y);
        p.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), cornerRadius, cornerRadius, leftCorners, rightCorners, leftCorners, rightCorners);
        p.closeSubPath();

        if (idx == value) {
            g.setColour(SoundfruuzeColor::lightBlue);
            g.fillPath(p);
        }

        g.setColour(SoundfruuzeColor::white);
        g.drawText(e, bounds, Justification::centred, false);
        idx++;
    }
}

void HorizontalTabs::mouseDown(const MouseEvent& event)
{
    auto newValue = (float(event.x) / getLocalBounds().getWidth() * options.size());
    if (value == newValue)
        return;
    value = newValue;
    fnAction();
    repaint();
}

// --------------------------------------------------

void VerticalTabs::paint(Graphics& g)
{
    g.setColour(colorOff);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    if (!options.size())
        return;

    auto bounds = getLocalBounds();
    bounds.expand(0, 0);
    auto optionHeight = bounds.getHeight() / options.size();

    size_t idx = 0;
    for (auto& e : options) {

        bounds.setHeight(optionHeight + 1);
        bounds.setY(idx * optionHeight);

        g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
        g.setFont(getLocalBounds().getHeight() / ((options.size() > 0) ? options.size() : 1) * .5);

        const float& x = bounds.getX();
        const float& y = bounds.getY();

        const float w = bounds.getWidth();
        const float h = bounds.getHeight();

        bool topCorners = (idx == 0);
        bool bottomCorners = (idx == (options.size() - 1));

        Path p;
        p.startNewSubPath(x, y);
        p.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight() - 1, cornerRadius, cornerRadius, topCorners, topCorners, bottomCorners, bottomCorners);
        p.closeSubPath();

        if (idx == value) {
            g.setColour(SoundfruuzeColor::lightBlue);
            g.fillPath(p);
        }

        g.setColour(SoundfruuzeColor::white);
        g.drawText(e, bounds, Justification::centred, false);
        idx++;
    }
}

void VerticalTabs::mouseDown(const MouseEvent& event)
{
    auto newValue = (float(event.y) / getLocalBounds().getHeight() * options.size());
    if (value == newValue)
        return;
    value = newValue;
    fnAction();
    repaint();
}

// --------------------------------------------------

void PagesView::addPage(WidgetPtr p)
{
    _pages.push_back(p);

    p->setBounds(getLocalBounds());
    addChildComponent(p.get());
    showPage(_currentPage);
}

void PagesView::showPage(const size_t& p)
{
    if (p >= _pages.size())
        return;

    if (p == _currentPage)
        return;

    if (_currentPage < _pages.size())
        _pages[_currentPage]->setVisible(false);

    _pages[p]->setBounds(getLocalBounds());
    _pages[p]->setVisible(true);

    _currentPage = p;
}

void PagesView::resized()
{
    juce::Component::resized();

    for (auto& e : _pages)
        e->setBounds(getLocalBounds());
}

// =============================================

void VerticalMultiTabsView::paint(Graphics& g)
{
    g.setColour(colorOff);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    if (!values.size())
        return;

    auto bounds = getLocalBounds();
    bounds.expand(0, 0);
    auto optionHeight = bounds.getHeight() / values.size();

    size_t idx = 0;
    for (auto e : values) {

        bounds.setHeight(optionHeight + 1);
        bounds.setY(idx * optionHeight);

        g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
        g.setFont(bounds.getHeight() * .5);

        const float& x = bounds.getX();
        const float& y = bounds.getY();

        const float w = bounds.getWidth();
        const float h = bounds.getHeight();

        bool topCorners = (idx == 0);
        bool bottomCorners = (idx == (values.size() - 1));

        Path p;
        p.startNewSubPath(x, y);
        p.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), cornerRadius, cornerRadius, topCorners, topCorners, bottomCorners, bottomCorners);
        p.closeSubPath();

        if (e) {
            g.setColour(SoundfruuzeColor::lightBlue);
            g.fillPath(p);
        }

        g.setColour(SoundfruuzeColor::white);
        g.drawText(text, bounds, Justification::centred, false);
        idx++;
    }
}

void VerticalMultiTabsView::mouseDown(const MouseEvent& event)
{
    auto newValue = (float(event.y) / getLocalBounds().getHeight() * values.size());
    //    if (values[newValue] == newValue)
    //        return;
    values[newValue] = !values[newValue];
    fnAction();
    repaint();
}

// ===============

void OptionalVerticalTabsView::paint(Graphics& g)
{
    g.setColour(colorOff);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    if (!options.size())
        return;

    auto bounds = getLocalBounds();
    bounds.expand(0, 0);
    auto optionHeight = bounds.getHeight() / options.size();

    size_t idx = 0;
    for (auto& e : options) {

        bounds.setHeight(optionHeight + 1);
        bounds.setY(idx * optionHeight);

        g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
        g.setFont(getLocalBounds().getHeight() / options.size() * .5);

        const float& x = bounds.getX();
        const float& y = bounds.getY();

        const float w = bounds.getWidth();
        const float h = bounds.getHeight();

        bool topCorners = (idx == 0);
        bool bottomCorners = (idx == (options.size() - 1));

        Path p;
        p.startNewSubPath(x, y);
        p.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), cornerRadius, cornerRadius, topCorners, topCorners, bottomCorners, bottomCorners);
        p.closeSubPath();

        if (idx == value) {
            g.setColour(SoundfruuzeColor::lightBlue);
            g.fillPath(p);
        }

        g.setColour(e.flag ? SoundfruuzeColor::darkerGray : SoundfruuzeColor::white);
        g.drawText(e.value, bounds, Justification::centred, false);
        idx++;
    }
}

void OptionalVerticalTabsView::mouseDown(const MouseEvent& event)
{
    auto newValue = (float(event.y) / getLocalBounds().getHeight() * options.size());
    if (value == newValue)
        return;
    value = newValue;
    fnAction();
    repaint();
}

// ===============

void HideableVerticalTabsView::paint(Graphics& g)
{
    g.setColour(colorOff);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

    if (!hidden.size())
        return;

    auto bounds = getLocalBounds();
    bounds.expand(0, 0);
    auto optionHeight = bounds.getHeight() / hidden.size();

    size_t idx = 0;
    for (auto e : hidden) {

        bounds.setHeight(optionHeight + 1);
        bounds.setY(idx * optionHeight);

        g.setFont(juce::Font("Helvetica Neue", "Thin", 14));
        g.setFont(bounds.getHeight() * .5);

        const float& x = bounds.getX();
        const float& y = bounds.getY();

        const float w = bounds.getWidth();
        const float h = bounds.getHeight();

        bool topCorners = (idx == 0);
        bool bottomCorners = (idx == (hidden.size() - 1));

        Path p;
        p.startNewSubPath(x, y);
        p.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), cornerRadius, cornerRadius, topCorners, topCorners, bottomCorners, bottomCorners);
        p.closeSubPath();

        if (!e) {
            if (idx == value) {
                g.setColour(SoundfruuzeColor::lightBlue);
                g.fillPath(p);
            }
            g.setColour(SoundfruuzeColor::white);
            g.drawText(text, bounds, Justification::centred, false);
        }

        idx++;
    }
}

void HideableVerticalTabsView::mouseDown(const MouseEvent& event)
{
    auto newValue = (float(event.y) / getLocalBounds().getHeight() * hidden.size());
    if (value == newValue)
        return;
    value = newValue;
    fnAction();
    repaint();
}

// =============================================

namespace ToolbarWidget {

RoundButtonPtr createCloseAllButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "X";
    ret->colorOff = SoundfruuzeColor::red;
    return ret;
}

BasicTogglePtr createPauseButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "||";
    return ret;
}

BasicTogglePtr createForcePauseButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "~";
    ret->colorOn = SoundfruuzeColor::orange;
    return ret;
}

WidgetPtr createTapButton()
{
    auto ret = std::make_shared<BasicButton>();
    ret->ButtonCommon::text = "Tap";
    return ret;
}

BasicTogglePtr createSnapButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "Snap";
    return ret;
}

BasicTogglePtr createThruButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "Thru";
    return ret;
}

BasicTogglePtr createZoomButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "Zoom";
    return ret;
}

HorizontalSliderPtr createInputLevelSlider()
{
    auto ret = std::make_shared<HorizontalSlider>();
    ret->setValueClass(getToolbarInputSliderValueClass());
    return ret;
}
HorizontalSliderPtr createOutputLevelSlider()
{
    auto ret = std::make_shared<HorizontalSlider>();
    ret->setValueClass(getToolbarOutputSliderValueClass());
    return ret;
}

HorizontalSliderPtr createBPMSlider()
{
    auto ret = std::make_shared<HorizontalSlider>();
    ret->setValueClass(getToolbarBPMSliderValueClass());
    return ret;
}

BasicTogglePtr createLimiterButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "Limiter";
    return ret;
}

BasicTogglePtr createSoftclipButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "Softclip";
    return ret;
}

HorizontalTabsPtr createBeatDivisionTabs()
{
    auto ret = std::make_shared<HorizontalTabs>();
    ret->options = { "1/1", "1/2", "1/3", "1/4", "1/6", "1/8" };
    return ret;
}

HorizontalTabsPtr createToolbarTabs()
{
    auto ret = std::make_shared<HorizontalTabs>();
    ret->options = { "Levels", "Timeline" };
    return ret;
}

RoundTogglePtr createReverseRoundButton()
{
    auto ret = std::make_shared<RoundToggle>();
    ret->ButtonCommon::text = "<<";
    ret->colorOff = SoundfruuzeColor::cyan;
    ret->colorOn = SoundfruuzeColor::cyan;
    return ret;
}

RoundButtonPtr createRetriggerRoundButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "|<";
    ret->colorOff = SoundfruuzeColor::yellow;
    return ret;
}

RoundTogglePtr createPauseRoundButton()
{
    auto ret = std::make_shared<RoundToggle>();
    ret->ButtonCommon::text = "||";
    ret->colorOff = SoundfruuzeColor::lime;
    ret->colorOn = SoundfruuzeColor::lime;
    return ret;
}

RoundToggleTriPtr createLanePauseRoundButton()
{
    auto ret = std::make_shared<RoundToggleTri>();
    ret->ButtonCommon::text = "||";
    ret->colorOff = SoundfruuzeColor::lime;
    ret->colorOn = SoundfruuzeColor::lime;
    return ret;
}

RoundToggleTriPtr createLooperLockSizeRoundButton()
{
    auto ret = std::make_shared<RoundToggleTri>();
    ret->ButtonCommon::text = "< >";
    ret->colorOff = SoundfruuzeColor::yellow;
    ret->colorOn = SoundfruuzeColor::yellow;
    return ret;
}

///> @deprecated
WidgetPtr createTransparentRoundButton();

// new side buttons:
RoundTogglePtr createRecordFromUpperRoundButton()
{
    auto ret = std::make_shared<RoundToggle>();
    ret->ButtonCommon::text = "R";
    ret->colorOff = SoundfruuzeColor::red;
    ret->colorOn = SoundfruuzeColor::red;
    return ret;
}
RoundTogglePtr createDisablePauseRoundButton()
{
    auto ret = std::make_shared<RoundToggle>();
    ret->ButtonCommon::text = "~";
    ret->colorOff = SoundfruuzeColor::orange;
    ret->colorOn = SoundfruuzeColor::orange;
    return ret;
}

RoundTogglePtr createMaximizeRoundButton()
{
    auto ret = std::make_shared<RoundToggle>();
    ret->ButtonCommon::text = "!";
    ret->colorOff = SoundfruuzeColor::green;
    ret->colorOn = SoundfruuzeColor::green;
    return ret;
}

///> @deprecated
RoundTogglePtr createNewLaneBelowButton()
{
    auto ret = std::make_shared<RoundToggle>();
    ret->ButtonCommon::text = "<<";
    ret->colorOff = SoundfruuzeColor::green;
    ret->colorOn = SoundfruuzeColor::green;
    return ret;
}

RoundTogglePtr createMuteRoundButton()
{
    auto ret = std::make_shared<RoundToggle>();
    ret->ButtonCommon::text = "M";
    ret->colorOff = SoundfruuzeColor::yellow;
    ret->colorOn = SoundfruuzeColor::yellow;
    return ret;
}

WidgetPtr createHoldButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->cornerRadius = 0;
    ret->colorOff = SoundfruuzeColor::gray;
    ret->colorOn = SoundfruuzeColor::lime;
    return ret;
}

BasicButtonPtr createHelpButton()
{
    auto ret = std::make_shared<BasicButton>();
    ret->ButtonCommon::text = "?";
    ret->colorOn = SoundfruuzeColor::gray;
    return ret;
}

RoundButtonPtr createCtrlDefaultButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "default";
    ret->colorOff = SoundfruuzeColor::green;
    return ret;
}
RoundButtonPtr createCtrlAllButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "all";
    ret->colorOff = SoundfruuzeColor::lightBlue;
    return ret;
}
RoundButtonPtr createCtrlNoneButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "none";
    ret->colorOff = SoundfruuzeColor::blue;
    return ret;
}


RoundButtonPtr createCtrlResetButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "reset";
    ret->colorOff = SoundfruuzeColor::red;
    return ret;
}

RoundButtonPtr createAddLaneButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "+";
    ret->colorOff = SoundfruuzeColor::orange;
    return ret;
}
RoundButtonPtr createRemoveLaneButton()
{
    auto ret = std::make_shared<RoundButton>();
    ret->ButtonCommon::text = "—";
    ret->colorOff = SoundfruuzeColor::cyan;
    return ret;
}

BasicTogglePtr createSnapshotSelectorButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "snapshots";
    ret->colorOff = SoundfruuzeColor::cyan;
    ret->colorOn = SoundfruuzeColor::orange;
    return ret;
}
BasicTogglePtr createRemoteSetupButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "setup";
    ret->colorOff = SoundfruuzeColor::cyan;
    ret->colorOn = SoundfruuzeColor::orange;
    return ret;
}

BasicTogglePtr createMIDILearnButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "midi learn";
    ret->colorOff = SoundfruuzeColor::grayTranslucent;
    ret->colorOn = SoundfruuzeColor::lime;
    return ret;
}


BasicButtonPtr createSnapshotResetButton()
{
    auto ret = std::make_shared<BasicButton>();
    ret->ButtonCommon::text = "reset";
    ret->colorOff = SoundfruuzeColor::orange;
        ret->colorOn = SoundfruuzeColor::orange;
    return ret;
}
BasicTogglePtr createSnapshotNumberButton(const size_t idx)
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = std::to_string(idx);
    ret->colorOff = SoundfruuzeColor::darkGray;
    ret->colorOn = SoundfruuzeColor::blue;
    return ret;
}

BasicTogglePtr createSnapshotWriteButton()
{
    auto ret = std::make_shared<BasicToggle>();
    ret->ButtonCommon::text = "write";
    ret->colorOff = SoundfruuzeColor::gray;
    ret->colorOn = SoundfruuzeColor::red;
    return ret;
}
}
