/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

#include "Widgets.hpp"

//==============================================================================
/**
*/
class CloudSeedJUCEEditor : public juce::AudioProcessorEditor,
                            public juce::AudioProcessorParameter::Listener,
                            public juce::AudioProcessorListener {
public:
    CloudSeedJUCEEditor(CloudSeedProcessor&);
    ~CloudSeedJUCEEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    VerticalLayoutView vLayout;
    std::array<HorizontalLayoutView, 3> hLayouts;

    std::array<Widgets::Group, 4> groups;

    Widgets::RandomSeedGroup randomSeeds;
    Widgets::ProgramGroup presetName;
    Widgets::InfoGroup parameterValue;

    CloudSeedProcessor& audioProcessor;

    juce::Label nameLabel;

    juce::Label infoLabel;

    //
    std::array<std::vector<std::shared_ptr<Widgets::ButtonSlider>>, 4> _buttons;
    std::array<std::vector<std::shared_ptr<Widgets::Knob>>, 4> _knobsUpper;
    std::array<std::vector<std::shared_ptr<Widgets::Knob>>, 4> _knobsLower;

    std::vector<std::shared_ptr<Widgets::ButtonSlider>> _randomSeedButtons;

    virtual void parameterValueChanged(int parameterIndex, float newValue) override;
    virtual void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override ;
    virtual void audioProcessorParameterChanged(AudioProcessor *processor,
                                                int parameterIndex,
                                                float newValue) override;
    virtual void audioProcessorChanged(juce::AudioProcessor *p,
                                       const ChangeDetails &details) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CloudSeedJUCEEditor)
};
