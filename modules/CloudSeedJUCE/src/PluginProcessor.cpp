/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "BinaryData.h"

#include "json.hpp"

using json = nlohmann::json;

const std::string ParameterName(const Parameter& p)
{
    if (p == Parameter::InputMix)
        return "Input Mix";
    if (p == Parameter::PreDelay)
        return "Predelay";

    if (p == Parameter::HighPass)
        return "High Pass";
    if (p == Parameter::LowPass)
        return "Low Pass";

    if (p == Parameter::TapCount)
        return "Tap Count";
    if (p == Parameter::TapLength)
        return "Tap Length";
    if (p == Parameter::TapGain)
        return "Tap Gain";
    if (p == Parameter::TapDecay)
        return "Tap Decay";

    if (p == Parameter::DiffusionEnabled)
        return "Diffusion Enabled";
    if (p == Parameter::DiffusionStages)
        return "Diffusion Stages";
    if (p == Parameter::DiffusionDelay)
        return "Diffusion Delay";
    if (p == Parameter::DiffusionFeedback)
        return "Diffusion Feedback";

    if (p == Parameter::LineCount)
        return "Line Count";
    if (p == Parameter::LineDelay)
        return "Line Delay";
    if (p == Parameter::LineDecay)
        return "Line Decay";

    if (p == Parameter::LateDiffusionEnabled)
        return "Late Diffusion Enabled";
    if (p == Parameter::LateDiffusionStages)
        return "Late Diffusion Stages";
    if (p == Parameter::LateDiffusionDelay)
        return "Late Diffusion Delay";
    if (p == Parameter::LateDiffusionFeedback)
        return "Late Diffusion Feedback";

    if (p == Parameter::PostLowShelfGain)
        return "Post LowShelf Gain";
    if (p == Parameter::PostLowShelfFrequency)
        return "Post LowShelf Frequency";
    if (p == Parameter::PostHighShelfGain)
        return "Post HighShelf Gain";
    if (p == Parameter::PostHighShelfFrequency)
        return "Post HighShelf Frequency";
    if (p == Parameter::PostCutoffFrequency)
        return "Post Cutoff Frequency";

    if (p == Parameter::EarlyDiffusionModAmount)
        return "Early Diffusion Mod Amount";
    if (p == Parameter::EarlyDiffusionModRate)
        return "Early Diffusion Mod Rate";

    if (p == Parameter::LineModAmount)
        return "Line Mod Amount";
    if (p == Parameter::LineModRate)
        return "Line Mod Rate";

    if (p == Parameter::LateDiffusionModAmount)
        return "Late Diffusion Mod Amount";
    if (p == Parameter::LateDiffusionModRate)
        return "Late Diffusion Mod Rate";

    if (p == Parameter::TapSeed)
        return "Tap Seed";
    if (p == Parameter::DiffusionSeed)
        return "Diffusion Seed";
    if (p == Parameter::DelaySeed)
        return "Delay Seed";
    if (p == Parameter::PostDiffusionSeed)
        return "Post Diffusion Seed";

    if (p == Parameter::CrossSeed)
        return "Cross Seed";

    if (p == Parameter::DryOut)
        return "Dry Out";
    if (p == Parameter::PredelayOut)
        return "Predelay Out";
    if (p == Parameter::EarlyOut)
        return "Early Out";
    if (p == Parameter::MainOut)
        return "Main Out";

    if (p == Parameter::HiPassEnabled)
        return "HiPass Enabled";
    if (p == Parameter::LowPassEnabled)
        return "LowPass Enabled";
    if (p == Parameter::LowShelfEnabled)
        return "LowShelf Enabled";
    if (p == Parameter::HighShelfEnabled)
        return "HighShelf Enabled";
    if (p == Parameter::CutoffEnabled)
        return "Cutoff Enabled";
    if (p == Parameter::LateStageTap)
        return "Late Stage Tap";

    if (p == Parameter::Interpolation)
        return "Interpolation";

    return "PNAME";
}

///> json keys
const std::string KeyName(const Parameter& p)
{
    if (p == Parameter::InputMix)
        return "InputMix";
    if (p == Parameter::PreDelay)
        return "PreDelay";

    if (p == Parameter::HighPass)
        return "HighPass";
    if (p == Parameter::LowPass)
        return "LowPass";

    if (p == Parameter::TapCount)
        return "TapCount";
    if (p == Parameter::TapLength)
        return "TapLength";
    if (p == Parameter::TapGain)
        return "TapGain";
    if (p == Parameter::TapDecay)
        return "TapDecay";

    if (p == Parameter::DiffusionEnabled)
        return "DiffusionEnabled";
    if (p == Parameter::DiffusionStages)
        return "DiffusionStages";
    if (p == Parameter::DiffusionDelay)
        return "DiffusionDelay";
    if (p == Parameter::DiffusionFeedback)
        return "DiffusionFeedback";

    if (p == Parameter::LineCount)
        return "LineCount";
    if (p == Parameter::LineDelay)
        return "LineDelay";
    if (p == Parameter::LineDecay)
        return "LineDecay";

    if (p == Parameter::LateDiffusionEnabled)
        return "LateDiffusion Enabled";
    if (p == Parameter::LateDiffusionStages)
        return "LateDiffusion Stages";
    if (p == Parameter::LateDiffusionDelay)
        return "LateDiffusion Delay";
    if (p == Parameter::LateDiffusionFeedback)
        return "LateDiffusion Feedback";

    if (p == Parameter::PostLowShelfGain)
        return "PostLowShelfGain";
    if (p == Parameter::PostLowShelfFrequency)
        return "PostLowShelfFrequency";
    if (p == Parameter::PostHighShelfGain)
        return "PostHighShelfGain";
    if (p == Parameter::PostHighShelfFrequency)
        return "PostHighShelfFrequency";
    if (p == Parameter::PostCutoffFrequency)
        return "PostCutoffFrequency";

    if (p == Parameter::EarlyDiffusionModAmount)
        return "EarlyDiffusionModAmount";
    if (p == Parameter::EarlyDiffusionModRate)
        return "EarlyDiffusionModRate";

    if (p == Parameter::LineModAmount)
        return "LineModAmount";
    if (p == Parameter::LineModRate)
        return "LineModRate";

    if (p == Parameter::LateDiffusionModAmount)
        return "LateDiffusionModAmount";
    if (p == Parameter::LateDiffusionModRate)
        return "LateDiffusionModRate";

    if (p == Parameter::TapSeed)
        return "TapSeed";
    if (p == Parameter::DiffusionSeed)
        return "DiffusionSeed";
    if (p == Parameter::DelaySeed)
        return "DelaySeed";
    if (p == Parameter::PostDiffusionSeed)
        return "PostDiffusionSeed";

    if (p == Parameter::CrossSeed)
        return "CrossSeed";

    if (p == Parameter::DryOut)
        return "DryOut";
    if (p == Parameter::PredelayOut)
        return "PredelayOut";
    if (p == Parameter::EarlyOut)
        return "EarlyOut";
    if (p == Parameter::MainOut)
        return "MainOut";

    if (p == Parameter::HiPassEnabled)
        return "HiPassEnabled";
    if (p == Parameter::LowPassEnabled)
        return "LowPassEnabled";
    if (p == Parameter::LowShelfEnabled)
        return "LowShelfEnabled";
    if (p == Parameter::HighShelfEnabled)
        return "HighShelfEnabled";
    if (p == Parameter::CutoffEnabled)
        return "CutoffEnabled";
    if (p == Parameter::LateStageTap)
        return "LateStageTap";

    if (p == Parameter::Interpolation)
        return "Interpolation";

    return "NONE";
}

class ReverbParameter : public juce::RangedAudioParameter {
    CloudSeed::ReverbController& controller;

    Parameter parameter;

    NormalisableRange<float> range;

public:
    ReverbParameter(CloudSeed::ReverbController& ctrl_, const Parameter& par_)
        : controller(ctrl_)
        , parameter(par_)
        , RangedAudioParameter(ParameterName(par_), ParameterName(par_))
    {
        range.start = controller.GetParameterMinimum(parameter);
        range.end = controller.GetParameterMaximum(parameter);
    }

    float getValue() const { return controller.GetRawParameter(parameter); }
    void setValue(float nv) { controller.SetParameter(parameter, nv); }; // unscaled!

    float getDefaultValue() const { return .5; }
    juce::String getName(int maximumStringLength) const { return ParameterName(parameter); }
    juce::String getLabel() const { return ParameterName(parameter); }
    float getValueForText(const String& text) const { return controller.GetNormalizedValue(parameter, std::stof(text.toStdString())); }
    juce::String getText(float normalisedValue, int) const
    {
        std::string ret = std::to_string(controller.GetScaledParameter(parameter));
        return ret;
    };

    virtual const NormalisableRange<float>& getNormalisableRange() const
    {
        return range;
    }

    float convertTo0to1(float v) const noexcept
    {
        return controller.GetNormalizedValue(parameter, v);
    }
    float convertFrom0to1(float v) const noexcept
    {
        return controller.GetScaledValue(parameter, v);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbParameter)
};

//==============================================================================
CloudSeedProcessor::CloudSeedProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
            )
    , state(*this, nullptr)
#endif
{
    AudioLib::ValueTables::Init();
    CloudSeed::FastSin::Init();

    // load factory presets
    factoryPresets.resize(9);

    factoryPresets[0].name = "Chorus Delay";
    factoryPresets[1].name = "Dull Echoes";
    factoryPresets[2].name = "Hyperplane";
    factoryPresets[3].name = "Medium Space";
    factoryPresets[4].name = "Noise in the Hallway";
    factoryPresets[5].name = "RubiKa Fields";
    factoryPresets[6].name = "Small Room";
    factoryPresets[7].name = "The 90s Are Back";
    factoryPresets[8].name = "Through the Looking Glass";

    factoryPresets[0].data = std::string(BinaryData::Chorus_Delay_json, BinaryData::Chorus_Delay_jsonSize);
    factoryPresets[1].data = std::string(BinaryData::Dull_Echoes_json, BinaryData::Dull_Echoes_jsonSize);
    factoryPresets[2].data = std::string(BinaryData::Hyperplane_json, BinaryData::Hyperplane_jsonSize);
    factoryPresets[3].data = std::string(BinaryData::Medium_Space_json, BinaryData::Medium_Space_jsonSize);
    factoryPresets[4].data = std::string(BinaryData::Noise_in_the_Hallway_json, BinaryData::Noise_in_the_Hallway_jsonSize);
    factoryPresets[5].data = std::string(BinaryData::RubiKa_Fields_json, BinaryData::RubiKa_Fields_jsonSize);
    factoryPresets[6].data = std::string(BinaryData::Small_Room_json, BinaryData::Small_Room_jsonSize);
    factoryPresets[7].data = std::string(BinaryData::The_90s_Are_Back_json, BinaryData::The_90s_Are_Back_jsonSize);
    factoryPresets[8].data = std::string(BinaryData::Through_the_Looking_Glass_json, BinaryData::Through_the_Looking_Glass_jsonSize);

    //
    controller.reset(new CloudSeed::ReverbController(44100));

    for (int i = 0; i < controller->GetParameterCount(); i++)
        controller->SetParameter((Parameter)i, 1);

    for (int i = 0; i < controller->GetParameterCount(); i++) {
        state.createAndAddParameter(std::make_unique<ReverbParameter>(*controller, (Parameter)i));
    }

    setCurrentProgram(1);
}

CloudSeedProcessor::~CloudSeedProcessor()
{
}

//==============================================================================
const juce::String CloudSeedProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CloudSeedProcessor::acceptsMidi() const
{
    return false;
}

bool CloudSeedProcessor::producesMidi() const
{
    return false;
}

bool CloudSeedProcessor::isMidiEffect() const
{
    return false;
}

double CloudSeedProcessor::getTailLengthSeconds() const
{
    return 10.0;
}

int CloudSeedProcessor::getNumPrograms()
{
    return factoryPresets.size(); ///< note: should be greater than 0 for some hosts
}

int CloudSeedProcessor::getCurrentProgram()
{
    return currentProgram;
}

void CloudSeedProcessor::setCurrentProgram(int index)
{
    if (index >= factoryPresets.size())
        return;

    currentProgram = index;
    auto e = factoryPresets[index].data;

    loadStateFromString(e);

}

// ---

void CloudSeedProcessor::loadStateFromString(const std::string& s){
    json j = json::object();
    j = json::parse(s);

    try {
        for (auto& v : j.items()) {
            auto k = v.key();

            for (int i = 0; i < (int)Parameter::Count; i++) {
                auto par = (Parameter)(i);
                auto s = KeyName(par);
                if (s.compare(k) == 0) {
                    float fval = v.value();
                    state.getParameter(ParameterName(par))->setValueNotifyingHost(fval);
                    fval = state.getParameter(ParameterName(par))->getValue();
                };
            }
        }
    } catch (std::exception&) {
        ///> @todo handle exception
    }
}

std::string CloudSeedProcessor::saveStateToString(){
    json j = json::object();

    for (int i = 0; i < (int)Parameter::Count; i++) {
        auto par = (Parameter)(i);
        auto s = KeyName(par);
        float v = state.getParameter(ParameterName(par))->getValue();
        j[s] = v;
    }
    return j.dump(4);

}

// ---

const juce::String CloudSeedProcessor::getProgramName(int index)
{
    if (index >= factoryPresets.size())
        return {};

    return factoryPresets[index].name;
}

void CloudSeedProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void CloudSeedProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    if (sampleRate < 1) return;
    if (samplesPerBlock < 1) return;

    ///> @todo work directly with 64 bit values

    std::vector<float> pars;
    for (int i = 0; i < controller->GetParameterCount(); i++)
        pars.push_back(controller->GetRawParameter((Parameter)i));

    auto count = getTotalNumInputChannels();

    controller->SetSamplerate(sampleRate);
    controller->SetChannelCount(count);

    ///> @todo validate buffer size
    /// there is currently a limitation in engine
    samplesPerBlock = 4096;

    doubleBuffer.resize(count);
    for (auto& e : doubleBuffer)
        e.resize(samplesPerBlock);

    for (int i = 0; i < controller->GetParameterCount(); i++)
        controller->SetParameter((Parameter)i, pars[i]);
}

void CloudSeedProcessor::releaseResources()
{
    ///> @todo
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CloudSeedProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{


    return true;

}
#endif

void CloudSeedProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    

    if (!controller)
        return;

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (!totalNumInputChannels)
        return;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    auto length = buffer.getNumSamples();

    //
    // assert(length < 4096);

    static const size_t numChannels = getTotalNumInputChannels();

    for (int ch = 0; ch < numChannels; ch++)
        for (int i = 0; i < length; i++) {
            doubleBuffer[ch][i] = buffer.getWritePointer(ch)[i];
        }

    static std::vector<double*> channelVec;
    if (channelVec.size() != numChannels)
        channelVec.resize(numChannels);
    static double** channels = channelVec.data();
    // double* channels[numChannels];

    for (int ch = 0; ch < numChannels; ch++)
        channels[ch] = doubleBuffer[ch].data();

    controller->Process(channels, channels, length);

    for (int ch = 0; ch < totalNumOutputChannels; ch++)
        for (int i = 0; i < length; i++) {
            buffer.getWritePointer(ch)[i] = channels[ch % totalNumInputChannels][i];

            // fix: remove NaN
            if (buffer.getWritePointer(ch)[i] != buffer.getWritePointer(ch)[i])
                buffer.getWritePointer(ch)[i] = 0;

            // fix: clip
            if (buffer.getWritePointer(ch)[i] > 1)
                buffer.getWritePointer(ch)[i] = 1;

            if (buffer.getWritePointer(ch)[i] < -1)
                buffer.getWritePointer(ch)[i] = -1;
        }

}

//==============================================================================

bool CloudSeedProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CloudSeedProcessor::createEditor()
{
    auto ret = new CloudSeedJUCEEditor(*this);
    addListener(ret);

    if (wrapperType == wrapperType_Standalone) {
//                LookAndFeel::getDefaultLookAndFeel().setColour(ResizableWindow::backgroundColourId,CurrentColorTheme().buttonTextOn );
                if(TopLevelWindow::getNumTopLevelWindows() == 1)
                {
                    TopLevelWindow* w = TopLevelWindow::getTopLevelWindow(0);

                    w->setColour(DocumentWindow::backgroundColourId, CurrentColorTheme().backgroundColor);

                }
    }

    return ret;
}

//==============================================================================

void CloudSeedProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto str = saveStateToString();
    destData.replaceWith(str.data(), str.size());

}

void CloudSeedProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::string str = std::string((char*)data, ((char*)data)+sizeInBytes);
    loadStateFromString(str);
}

//==============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CloudSeedProcessor();
}
