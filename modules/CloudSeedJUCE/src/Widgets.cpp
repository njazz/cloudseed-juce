#include "Widgets.hpp"

void LayoutViewCommon::setLayout(const LayoutData& d)
{
    _layoutData = d;
    resized();
}

void LayoutViewCommon::addSubview(juce::Component* c)
{
    _subviews.push_back(c);

    addChildComponent(c);
    c->setVisible(true);
    resized();
}

//
void HorizontalLayoutView::resized()
{
    auto result = _layoutData.calculate(getLocalBounds().getWidth());
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

    auto result = _layoutData.calculate(getLocalBounds().getHeight());
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
namespace Widgets {

void ButtonSlider::paint(juce::Graphics& g)
{
    juce::Path p;
    auto b = getLocalBounds();

    if ((max-min) > 1) {
        b.setWidth(b.getWidth() * value);
    }
    p.addRoundedRectangle(b.toFloat(), 5);

    juce::Path p0;
    p0.addRoundedRectangle(getLocalBounds().toFloat(), 5.0f);
    g.setColour(CurrentColorTheme().buttonBackgroundColor);
    g.fillPath(p0);


    g.setColour((value || ((max-min) > 1)) ? CurrentColorTheme().buttonOnBackground : CurrentColorTheme().buttonOffBackground);
    g.fillPath(p);

    g.setFont(12.0f);
    g.setColour(CurrentColorTheme().defaultTextColor);
    g.drawFittedText(fnDisplay(value), getLocalBounds().toNearestInt(),juce::Justification::centred,1 );
}

void ButtonSlider::mouseDown(const juce::MouseEvent& e)
{
    if ((max-min)!=1) return;

    value = !value;
    repaint();
    fnAction();
}

void ButtonSlider::mouseDrag(const juce::MouseEvent &event) {
    if ((max-min)==1) return;

    auto nv = float(event.getPosition().x) / getWidth();
    if (nv<0) nv = 0;
    if (nv>1) nv=1;
    value = nv;//min + nv* (max-min);
    repaint();
    fnAction();
}

// ---

void Knob::paint(juce::Graphics& g)
{
    g.setColour(CurrentColorTheme().buttonOffBackground);

    float d = getLocalBounds().getWidth() - getLocalBounds().getHeight();
//    if (d<16) d=16;
//    d *= -1;
    d += 0;

    auto w = (getLocalBounds().getHeight()< getLocalBounds().getWidth()) ? getLocalBounds().getHeight() : getLocalBounds().getWidth() - d;
    w -= 32;
    auto h = w;

    // if (d<24) d=24;
    auto x0 = getLocalBounds().getWidth() - w;

    float s = .6 * 2* M_PI;
    float e = (1.4) * 2*M_PI;

    // gray arc
    juce::Path p;
    p.addArc(x0 * .5,8,w,h,s,e, true);
    g.strokePath(p,juce::PathStrokeType(2.0f));

    // blue arc
    g.setColour(CurrentColorTheme().knobColor);
    e -= .8*(1-value)*2*M_PI;
    p.clear();
    p.addArc(x0 * .5 + 1. ,8 + 1. ,w - 2,h - 2,s,e, true);
    g.strokePath(p,juce::PathStrokeType(4.0f));

    // label
    g.setFont(CurrentColorTheme().font);
    g.setFont(12.0f);
    g.setColour(CurrentColorTheme().defaultTextColor);
    auto b = getLocalBounds();
    b.setHeight(24);
    b.setPosition(x0*.5,getLocalBounds().getHeight()-24);
    b.setWidth(w);
    g.drawFittedText(name,b.toNearestInt(),juce::Justification::centredBottom,2,0);

    // center text
    g.setFont(9);

    // value label background
    auto lw = g.getCurrentFont().getStringWidthFloat(fnDisplay(value)) + 10;
    g.setColour(CurrentColorTheme().knobValueBackground);
    auto w2 = getLocalBounds().getWidth();
    g.fillRoundedRectangle(w2 * .5-lw * .5, getLocalBounds().getHeight()-32, lw, 9, 5);

    g.setColour(CurrentColorTheme().defaultTextColor);
    b.setPosition(x0*.5,getLocalBounds().getHeight()-32);
    g.drawFittedText(fnDisplay(value),b.toNearestInt(),juce::Justification::centredTop,2,0);

}

void Knob::mouseDown(const juce::MouseEvent& e)
{
    _lastPosition = e.getPosition();
}

void Knob::mouseDrag(const juce::MouseEvent & e) {
    value -= float(e.getPosition().y - _lastPosition.y) / getHeight() ;
    _lastPosition = e.getPosition();

    if (value<0) value = 0;
    if (value>1) value = 1;
    repaint();

    fnAction();
}

void Knob::mouseDoubleClick(const juce::MouseEvent&)
{
    value =fnGetDefault();
    repaint();
}

// ---

Group::Group(){
    CSJ_LAYOUT_BEGIN(_main)

    CSJ_LAYOUT_MARGIN_PX(28)
    CSJ_LAYOUT_RATIO(1)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_RATIO(3)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_RATIO(3)

    CSJ_LAYOUT_END_PX(5);

    setLayout(_main());

    for(auto& e: rows)
        addSubview(&e);
}

RandomSeedGroup::RandomSeedGroup(){
    CSJ_LAYOUT_BEGIN(_main)

    CSJ_LAYOUT_MARGIN_PX(12)
    CSJ_LAYOUT_RATIO(1)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_RATIO(1)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_RATIO(1)

    CSJ_LAYOUT_END_PX(5);

    setLayout(_main());

    addSubview(&labels);
    addSubview(&buttons);
//    for(auto& e: rows)
//        addSubview(&e);
}

// --

void BaseGroup::paint(juce::Graphics& g)
{
    juce::Path p;

    auto b = getLocalBounds().toFloat();
    p.addRoundedRectangle(b, 10.0f);

    g.setColour(CurrentColorTheme().groupBackgroundColor);
    g.fillPath(p,AffineTransform());

//    g.strokePath(p,juce::PathStrokeType(1.5f));
//    p.clear();
//    b.reduce(1,1);
//    p.addRoundedRectangle(b, 10.0f);
//    g.setColour(CurrentColorTheme().backgroundColor);
//    g.fillPath(p,AffineTransform());


    g.setFont(CurrentColorTheme().font);
    g.setFont(16.0f);
    g.setColour(CurrentColorTheme().defaultTextColor);

    b = getLocalBounds().toFloat();
    b.setHeight(25);
    g.drawFittedText(name,b.toNearestInt(),juce::Justification::centred,1);
}


void InfoGroup::paint(juce::Graphics& g)
{
    BaseGroup::paint(g);

    g.setFont(CurrentColorTheme().font);
    g.setFont(24.0f);
    g.setColour(CurrentColorTheme().defaultTextColor);
    auto b = getLocalBounds();
    b.setPosition(0,0);
    g.drawFittedText(value,b.toNearestInt(),juce::Justification::centred,2);
}

void ProgramGroup::paint(juce::Graphics &g) {
    InfoGroup::paint(g);

    g.setFont(CurrentColorTheme().font);
    g.setFont(16.0f);
    g.setColour(CurrentColorTheme().defaultTextColor);
    auto b = getLocalBounds();
    b.setPosition(5,0);
    b.setWidth(b.getWidth()-10);

    g.drawFittedText(" <",b.toNearestInt(),juce::Justification::left,1);
    g.drawFittedText("> ",b.toNearestInt(),juce::Justification::right,1);
}

void ProgramGroup::mouseDown(const juce::MouseEvent &event) {
    if (event.getMouseDownPosition().x<20) fnDec();
    if (event.getMouseDownPosition().x>(getWidth()-20)) fnInc();
}

}
