/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

#include "InfoString.hpp"

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

//==============================================================================
CloudSeedJUCEEditor::CloudSeedJUCEEditor(CloudSeedProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
{
    setSize(1000, 400);

    addAndMakeVisible(&vLayout);

    nameLabel.setText("CloudSeed [JUCE]", juce::NotificationType::dontSendNotification);
    nameLabel.setFont(CurrentColorTheme().font);
    nameLabel.setJustificationType(juce::Justification::centredLeft);
    nameLabel.setColour(juce::Label::textColourId, CurrentColorTheme().defaultTextColor);

    infoLabel.setText(infoString, juce::NotificationType::dontSendNotification);
    infoLabel.setFont(CurrentColorTheme().fontSmall);
    infoLabel.setJustificationType(juce::Justification::centredRight);
    infoLabel.setColour(juce::Label::textColourId, CurrentColorTheme().defaultTextColor);

    CSJ_LAYOUT_BEGIN(_vertical)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_PERCENT(15)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_PERCENT(60)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_PERCENT(25)

    CSJ_LAYOUT_END_PX(5)

    vLayout.addSubview(&hLayouts[0]);
    vLayout.addSubview(&hLayouts[1]);
    vLayout.addSubview(&hLayouts[2]);
    vLayout.setLayout(_vertical());

    // top

    CSJ_LAYOUT_BEGIN(_top)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_PERCENT(50)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_FILL()

    CSJ_LAYOUT_END_PX(0)

    hLayouts[0].addSubview(&nameLabel);
    hLayouts[0].addSubview(&infoLabel);
    hLayouts[0].setLayout(_top());

    // middle

    CSJ_LAYOUT_BEGIN(_middle)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_PERCENT(25)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(25)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(25)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(22)

    CSJ_LAYOUT_END_PX(5)

    groups[0].name = "Main & Filters";
    groups[1].name = "Early Reflections";
    groups[2].name = "Late Reverberation";
    groups[3].name = "Equalization";

    for (auto& e : groups)
        hLayouts[1].addSubview(&e);

    hLayouts[1].setLayout(_middle());

    // bottom

    randomSeeds.name = "Random Seeds";
    presetName.name = "Program Bank";
    parameterValue.name = "No parameter selected";

    CSJ_LAYOUT_BEGIN(_bottom)

    CSJ_LAYOUT_MARGIN_PX(5)
    CSJ_LAYOUT_PERCENT(25)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(25)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(48)

    CSJ_LAYOUT_END_PX(5)

    hLayouts[2].addSubview(&randomSeeds);
    hLayouts[2].addSubview(&presetName);
    hLayouts[2].addSubview(&parameterValue);

    hLayouts[2].setLayout(_bottom());

    //
    CSJ_LAYOUT_BEGIN(_2buttons)

    CSJ_LAYOUT_MARGIN_FILL()
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_END_PERCENT(1)

    //

    CSJ_LAYOUT_BEGIN(_3buttons)

    CSJ_LAYOUT_MARGIN_FILL()
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_END_PERCENT(1)

    //

    CSJ_LAYOUT_BEGIN(_4buttons)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_PERCENT(23.75)

    CSJ_LAYOUT_END_PERCENT(1)

    //

    CSJ_LAYOUT_BEGIN(_3eq)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_RATIO(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_RATIO(23.75)

    CSJ_LAYOUT_MARGIN_PERCENT(1)
    CSJ_LAYOUT_RATIO(23.75)

    CSJ_LAYOUT_END_PERCENT(1)

    // ---

    for (auto& e : groups) {
        e.rows[1].setLayout(_4buttons());
        e.rows[2].setLayout(_4buttons());
    }

    groups[0].rows[0].setLayout(_2buttons());
    groups[1].rows[0].setLayout(_2buttons());
    groups[2].rows[0].setLayout(_4buttons());

    groups[3].rows[0].setLayout(_3eq());
    groups[3].rows[1].setLayout(_3eq());
    groups[3].rows[2].setLayout(_3eq());

    randomSeeds.labels.setLayout(_4buttons());
    randomSeeds.buttons.setLayout(_4buttons());

    // ---

    auto MakeKnob = [&](const Parameter& p) {
        std::shared_ptr<Widgets::Knob> ret = std::make_shared<Widgets::Knob>();

        ret->name = ParameterName(p);
        ret->parameterIndex = (int)p;

        auto name = ret->name;

        audioProcessor.state.getParameter(name)->addListener(this);

        auto value = ret->value;
        auto* THIS = this;
        ret->fnAction = [=]() {
//            juce::MessageManagerLock l;

            THIS->audioProcessor.state.getParameter(name)->beginChangeGesture();
            THIS->audioProcessor.state.getParameter(name)->setValueNotifyingHost(ret->value);
            THIS->audioProcessor.state.getParameter(name)->endChangeGesture();

            THIS->parameterValue.name = name;
            THIS->parameterValue.repaint();

            auto pVal = THIS->audioProcessor.state.getParameter(name)->getValue();
            THIS->parameterValue.value = THIS->audioProcessor.state.getParameter(name)->getText(pVal, 255).toStdString();
        };

        ret->fnDisplay = [=](const float& v){
            auto r = THIS->audioProcessor.state.getParameter(name)->getNormalisableRange();
            auto n = r.convertFrom0to1(v);
            return to_string_with_precision(n,2);
        };

        ret->value = THIS->audioProcessor.state.getParameter(name)->getValue();
        ret->repaint();

//        printf("param: %s v: %f\n", name.c_str(), ret->value);

        return ret;
    };

    auto MakeButton
        = [&](const Parameter& p) {
              std::shared_ptr<Widgets::ButtonSlider> ret = std::make_shared<Widgets::ButtonSlider>();

              ret->name = ParameterName(p);
              ret->parameterIndex = (int)p;

              auto name = ret->name;

              audioProcessor.state.getParameter(name)->addListener(this);

              auto value = ret->value;
              auto* THIS = this;
              ret->fnAction = [=]() {
//                  juce::MessageManagerLock l;

                  THIS->audioProcessor.state.getParameter(name)->beginChangeGesture();
                  THIS->audioProcessor.state.getParameter(name)->setValueNotifyingHost(ret->value);
                  THIS->audioProcessor.state.getParameter(name)->endChangeGesture();

                  THIS->parameterValue.name = name;
                  THIS->parameterValue.repaint();

                  auto pVal = THIS->audioProcessor.state.getParameter(name)->getValue();
                  THIS->parameterValue.value = THIS->audioProcessor.state.getParameter(name)->getText(pVal, 255).toStdString();
              };

              auto r = audioProcessor.state.getParameter(name)->getNormalisableRange();
              ret->min = r.convertFrom0to1(0);
              ret->max = r.convertFrom0to1(1);
              // printf("%s %i %i\n", name.c_str(), ret->min, ret->max);

              if ((ret->max - ret->min) != 1)
              {
                  auto min = ret->min;
                  auto max = ret->max;
                  ret->fnDisplay = [=](const float& value){
                      return std::to_string(int(value * (max-min)+min));
                  };
              }

              ret->value = audioProcessor.state.getParameter(name)->getValue();
              ret->repaint();

              return ret;
          };

    //

    //    auto buttons2 = {MakeButton(Parameter::HighPass),
    //                         MakeButton(Parameter::HighPass),
    //                     MakeButton(Parameter::HighPass),
    //                         MakeButton(Parameter::LowPassEnabled)
    //                         };

    //    auto buttons3 = {MakeButton(Parameter::HighPass),
    //                     MakeButton(Parameter::HighPass),
    //                     MakeButton(Parameter::HighPass),
    //                     MakeButton(Parameter::LowPassEnabled)
    //    };


    //
    _buttons[0] = {
        //        MakeButton(Parameter::PreDelay),
        MakeButton(Parameter::HiPassEnabled),
        MakeButton(Parameter::LowPassEnabled)
    };

    _knobsUpper[0] = { MakeKnob(Parameter::InputMix),
        MakeKnob(Parameter::PreDelay),
        MakeKnob(Parameter::HighPass),
        MakeKnob(Parameter::LowPass) };

    _knobsLower[0] = { MakeKnob(Parameter::DryOut),
        MakeKnob(Parameter::PredelayOut),
        MakeKnob(Parameter::EarlyOut),
        MakeKnob(Parameter::MainOut) };

    //
    _buttons[1] = {
        MakeButton(Parameter::DiffusionEnabled),
        MakeButton(Parameter::DiffusionStages)
    };

    _knobsUpper[1] = { MakeKnob(Parameter::TapCount),
        MakeKnob(Parameter::TapLength),
        MakeKnob(Parameter::DiffusionDelay),
        MakeKnob(Parameter::EarlyDiffusionModAmount) };

    _knobsLower[1] = { MakeKnob(Parameter::TapGain),
        MakeKnob(Parameter::TapDecay),
        MakeKnob(Parameter::DiffusionFeedback),
        MakeKnob(Parameter::EarlyDiffusionModRate) };

    //
    _buttons[2] = {
        MakeButton(Parameter::LateStageTap),
        MakeButton(Parameter::LineCount),
        MakeButton(Parameter::LateDiffusionEnabled),
        MakeButton(Parameter::Interpolation)
    };

    _knobsUpper[2] = { MakeKnob(Parameter::LateDiffusionStages),
        MakeKnob(Parameter::LineModAmount),
        MakeKnob(Parameter::LateDiffusionDelay),
        MakeKnob(Parameter::LateDiffusionModAmount) };

    _knobsLower[2] = { MakeKnob(Parameter::LineDecay),
        MakeKnob(Parameter::LineModRate),
        MakeKnob(Parameter::LateDiffusionFeedback),
        MakeKnob(Parameter::LateDiffusionModRate) };
    //
    _knobsUpper[3] = { MakeKnob(Parameter::PostLowShelfFrequency),
        MakeKnob(Parameter::PostHighShelfFrequency),
        MakeKnob(Parameter::PostCutoffFrequency) };

    _knobsLower[3] = { MakeKnob(Parameter::PostLowShelfGain),
        MakeKnob(Parameter::PostHighShelfGain),
        MakeKnob(Parameter::CrossSeed) };

    _buttons[3] = { MakeButton(Parameter::LowShelfEnabled),
        MakeButton(Parameter::HighShelfEnabled),
        MakeButton(Parameter::CutoffEnabled) };

    //
    _randomSeedButtons = {
        MakeButton(Parameter::TapSeed),
        MakeButton(Parameter::DiffusionSeed),
        MakeButton(Parameter::DelaySeed),
        MakeButton(Parameter::PostDiffusionSeed)
    };

    // ---

    for (auto& e : _buttons[3])
        groups[3].rows[0].addSubview(e.get());

    for (auto& e : _randomSeedButtons)
        randomSeeds.buttons.addSubview(e.get());

    //
    auto setupGroup = [&](const int idx) {
        for (auto& e : _buttons[idx])
            groups[idx].rows[0].addSubview(e.get());
        for (auto& e : _knobsUpper[idx])
            groups[idx].rows[1].addSubview(e.get());
        for (auto& e : _knobsLower[idx])
            groups[idx].rows[2].addSubview(e.get());
    };

    for (int i = 0; i < 4; i++)
        setupGroup(i);

    // ---

    for (int i=0;i<(int)Parameter::Count;i++){
        parameterValueChanged(i, p.state.getParameter(ParameterName((Parameter)i))->getValue());
    }

    // temporary fix:
    p.setCurrentProgram(p.getCurrentProgram());
    presetName.value = audioProcessor.getProgramName(audioProcessor.getCurrentProgram()).toStdString();
    presetName.repaint();

    //
    presetName.fnDec = [&](){
        auto idx = audioProcessor.getCurrentProgram();
        if (idx==0)
            return;
        audioProcessor.setCurrentProgram(--idx);

        // fix:
       audioProcessorChanged(nullptr);
    };

    presetName.fnInc = [&](){
        auto idx = audioProcessor.getCurrentProgram();
        if (idx==(audioProcessor.getNumPrograms())) return;
        audioProcessor.setCurrentProgram(++idx);

        // fix:
        audioProcessorChanged(nullptr);
    };

}

CloudSeedJUCEEditor::~CloudSeedJUCEEditor()
{
    for (int i=0;i<(int)Parameter::Count;i++){
        audioProcessor.state.getParameter(ParameterName((Parameter)i))->removeListener(this);
    }
    audioProcessor.removeListener(this);
}

//==============================================================================
void CloudSeedJUCEEditor::paint(juce::Graphics& g)
{
    g.fillAll(CurrentColorTheme().backgroundColor);
}

void CloudSeedJUCEEditor::resized()
{
    vLayout.setBounds(getLocalBounds());
}


// =====

 void CloudSeedJUCEEditor::parameterValueChanged(int parameterIndex, float newValue)
{
    //        juce::MessageManagerLock l;

    for (auto b : _buttons)
        for (auto e : b) {
            if (e->parameterIndex == parameterIndex) {
                juce::MessageManager::callAsync([=](){
                    e->value = newValue;
                    e->repaint();
                });

            }
        }

    for (auto b : _knobsUpper)
        for (auto e : b) {
            if (e->parameterIndex == parameterIndex) {
                juce::MessageManager::callAsync([=](){
                    e->value = newValue;
                    e->repaint();
                });
            }
        }

    for (auto b : _knobsLower)
        for (auto e : b) {
            if (e->parameterIndex == parameterIndex) {
                juce::MessageManager::callAsync([=](){
                    e->value = newValue;
                    e->repaint();
                });
            }
        }
};

 void CloudSeedJUCEEditor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)  {

};

 void CloudSeedJUCEEditor::audioProcessorParameterChanged(AudioProcessor* processor,
                                            int parameterIndex,
                                            float newValue)  { }

 void CloudSeedJUCEEditor::audioProcessorChanged(juce::AudioProcessor* p)
{
    auto sp = Component::SafePointer<CloudSeedJUCEEditor> (this);

    juce::MessageManager::callAsync([sp](){
        if (sp == nullptr)
            return;

        auto THIS = sp.getComponent();

        THIS->presetName.value = THIS->audioProcessor.getProgramName(THIS->audioProcessor.getCurrentProgram()).toStdString();
        THIS->presetName.repaint();
    });
}


