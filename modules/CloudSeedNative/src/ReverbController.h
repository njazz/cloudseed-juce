
#ifndef REVERBCONTROLLER
#define REVERBCONTROLLER

#include <vector>
#include "Default.h"
#include "Parameter.h"
#include "ReverbChannel.h"

#include "ReverbController.h"
#include "AudioLib/ValueTables.h"
#include "AllpassDiffuser.h"
#include "MultitapDiffuser.h"
#include "Utils.h"

namespace CloudSeed
{
	class ReverbController
	{
	private:
		static const int bufferSize = 4096; // just make it huge by default...
		int samplerate;

//		ReverbChannel channelL;
//		ReverbChannel channelR;

//		double leftChannelIn[bufferSize];
//		double rightChannelIn[bufferSize];
//		double leftLineBuffer[bufferSize];
//		double rightLineBuffer[bufferSize];

		double parameters[(int)Parameter::Count];

        std::vector<std::unique_ptr<ReverbChannel>> channels;

        std::vector<double*> channelIns = {};
        std::vector<double*> lineBuffers = {};

	public:
		ReverbController(int samplerate)
//            : channelL(bufferSize, samplerate, 0)
//            , channelR(bufferSize, samplerate, 1)
		{
			this->samplerate = samplerate;
            SetChannelCount(2);

            for (int i=0; i<(int)Parameter::Count;i++ ){
                parameters[i] = 0;
            }
		}

        ReverbController(const ReverbController&) = delete;
        ReverbController& operator=(const ReverbController&) = delete;

        // added:
        void SetChannelCount(const size_t&s){
            channels.clear();
            channels.resize(s);

            for (auto& c: channelIns){
                if (c) delete[] c;
            }
            for (auto& c: lineBuffers){
                if (c) delete[] c;
            }

            channelIns.resize(s);
            lineBuffers.resize(s);

            auto idx = 0;
            for (auto& c: channels){
                c.reset(new ReverbChannel(bufferSize, samplerate, idx));
                idx++;
            }

            for (auto& c: channelIns){
                c = new double[4096];
                for (int i=0;i<4096;i++)
                    c[i] = 0;

            }
            for (auto& c: lineBuffers){
                c = new double[4096];
                for (int i=0;i<4096;i++)
                    c[i] = 0;

            }

        }
        const size_t GetChannelCount(){
            return channels.size();
        }

		int GetSamplerate()
		{
			return samplerate;
		}

		void SetSamplerate(int samplerate)
		{
			this->samplerate = samplerate;

            for (auto& e: channels)
                e->SetSamplerate(samplerate);
		}

		int GetParameterCount()
		{
			return (int)Parameter::Count;
		}

		double* GetAllParameters()
		{
			return parameters;
		}

        double GetScaledValue(Parameter param, const float normValue)
		{
            // new:
            const auto GetValue = [=](Parameter){ return normValue; };

			switch (param)
			{
				// Input
            case Parameter::InputMix:                  return GetValue(Parameter::InputMix);
            case Parameter::PreDelay:                  return (int)(GetValue(Parameter::PreDelay) * 1000);

            case Parameter::HighPass:                  return 20 + ValueTables::Get(GetValue(Parameter::HighPass), ValueTables::Response4Oct) * 980;
            case Parameter::LowPass:                   return 400 + ValueTables::Get(GetValue(Parameter::LowPass), ValueTables::Response4Oct) * 19600;

				// Early
            case Parameter::TapCount:                  return 1 + (int)(GetValue(Parameter::TapCount) * (MultitapDiffuser::MaxTaps - 1));
            case Parameter::TapLength:                 return (int)(GetValue(Parameter::TapLength) * 500);
            case Parameter::TapGain:                   return ValueTables::Get(GetValue(Parameter::TapGain), ValueTables::Response2Dec);
            case Parameter::TapDecay:                  return GetValue(Parameter::TapDecay);

            case Parameter::DiffusionEnabled:          return GetValue(Parameter::DiffusionEnabled) < 0.5 ? 0.0 : 1.0;
            case Parameter::DiffusionStages:           return 1 + (int)(GetValue(Parameter::DiffusionStages) * (AllpassDiffuser::MaxStageCount - 0.001));
            case Parameter::DiffusionDelay:            return (int)(10 + GetValue(Parameter::DiffusionDelay) * 90);
            case Parameter::DiffusionFeedback:         return GetValue(Parameter::DiffusionFeedback);

				// Late
            case Parameter::LineCount:                 return 1 + (int)(GetValue(Parameter::LineCount) * 11.999);
            case Parameter::LineDelay:                 return (int)(20.0 + ValueTables::Get(GetValue(Parameter::LineDelay), ValueTables::Response2Dec) * 980);
            case Parameter::LineDecay:                 return 0.05 + ValueTables::Get(GetValue(Parameter::LineDecay), ValueTables::Response3Dec) * 59.95;

            case Parameter::LateDiffusionEnabled:      return GetValue(Parameter::LateDiffusionEnabled) < 0.5 ? 0.0 : 1.0;
            case Parameter::LateDiffusionStages:       return 1 + (int)(GetValue(Parameter::LateDiffusionStages) * (AllpassDiffuser::MaxStageCount - 0.001));
            case Parameter::LateDiffusionDelay:        return (int)(10 + GetValue(Parameter::LateDiffusionDelay) * 90);
            case Parameter::LateDiffusionFeedback:     return GetValue(Parameter::LateDiffusionFeedback);

				// Frequency Response
            case Parameter::PostLowShelfGain:          return ValueTables::Get(GetValue(Parameter::PostLowShelfGain), ValueTables::Response2Dec);
            case Parameter::PostLowShelfFrequency:     return 20 + ValueTables::Get(GetValue(Parameter::PostLowShelfFrequency), ValueTables::Response4Oct) * 980;
            case Parameter::PostHighShelfGain:         return ValueTables::Get(GetValue(Parameter::PostHighShelfGain), ValueTables::Response2Dec);
            case Parameter::PostHighShelfFrequency:    return 400 + ValueTables::Get(GetValue(Parameter::PostHighShelfFrequency), ValueTables::Response4Oct) * 19600;
            case Parameter::PostCutoffFrequency:       return 400 + ValueTables::Get(GetValue(Parameter::PostCutoffFrequency), ValueTables::Response4Oct) * 19600;

				// Modulation
            case Parameter::EarlyDiffusionModAmount:   return GetValue(Parameter::EarlyDiffusionModAmount) * 2.5;
            case Parameter::EarlyDiffusionModRate:     return ValueTables::Get(GetValue(Parameter::EarlyDiffusionModRate), ValueTables::Response2Dec) * 5;
            case Parameter::LineModAmount:             return GetValue(Parameter::LineModAmount) * 2.5;
            case Parameter::LineModRate:               return ValueTables::Get(GetValue(Parameter::LineModRate), ValueTables::Response2Dec) * 5;
            case Parameter::LateDiffusionModAmount:    return GetValue(Parameter::LateDiffusionModAmount) * 2.5;
            case Parameter::LateDiffusionModRate:      return ValueTables::Get(GetValue(Parameter::LateDiffusionModRate), ValueTables::Response2Dec) * 5;

				// Seeds
            case Parameter::TapSeed:                   return (int)std::floor(GetValue(Parameter::TapSeed) * 1000000 + 0.001);
            case Parameter::DiffusionSeed:             return (int)std::floor(GetValue(Parameter::DiffusionSeed) * 1000000 + 0.001);
            case Parameter::DelaySeed:                 return (int)std::floor(GetValue(Parameter::DelaySeed) * 1000000 + 0.001);
            case Parameter::PostDiffusionSeed:         return (int)std::floor(GetValue(Parameter::PostDiffusionSeed) * 1000000 + 0.001);

				// Output
            case Parameter::CrossSeed:                 return GetValue(Parameter::CrossSeed);

            case Parameter::DryOut:                    return ValueTables::Get(GetValue(Parameter::DryOut), ValueTables::Response2Dec);
            case Parameter::PredelayOut:               return ValueTables::Get(GetValue(Parameter::PredelayOut), ValueTables::Response2Dec);
            case Parameter::EarlyOut:                  return ValueTables::Get(GetValue(Parameter::EarlyOut), ValueTables::Response2Dec);
            case Parameter::MainOut:                   return ValueTables::Get(GetValue(Parameter::MainOut), ValueTables::Response2Dec);

				// Switches
            case Parameter::HiPassEnabled:             return GetValue(Parameter::HiPassEnabled) < 0.5 ? 0.0 : 1.0;
            case Parameter::LowPassEnabled:            return GetValue(Parameter::LowPassEnabled) < 0.5 ? 0.0 : 1.0;
            case Parameter::LowShelfEnabled:           return GetValue(Parameter::LowShelfEnabled) < 0.5 ? 0.0 : 1.0;
            case Parameter::HighShelfEnabled:          return GetValue(Parameter::HighShelfEnabled) < 0.5 ? 0.0 : 1.0;
            case Parameter::CutoffEnabled:             return GetValue(Parameter::CutoffEnabled) < 0.5 ? 0.0 : 1.0;
            case Parameter::LateStageTap:			   return GetValue(Parameter::LateStageTap) < 0.5 ? 0.0 : 1.0;

				// Effects
            case Parameter::Interpolation:			   return GetValue(Parameter::Interpolation) < 0.5 ? 0.0 : 1.0;

			default: return 0.0;
			}

			return 0.0;
		}

        // new
        double GetNormalizedValue(Parameter param, const float scaledValue){
            auto range = (GetParameterMaximum(param) - GetParameterMinimum(param));
            if (range==0) return 0;

            return (scaledValue - GetParameterMinimum(param)) / range;
        }

        double GetScaledParameter(Parameter param){
            return GetScaledValue(param, P(param));
        }

        // new
        double GetRawParameter(Parameter param){
            return P(param);
        }

        const double GetParameterMinimum(Parameter param){
            return GetScaledValue(param, 0);
        }
        const double GetParameterMaximum(Parameter param){
            return GetScaledValue(param, 1);
        }

		void SetParameter(Parameter param, double value)
		{
			parameters[(int)param] = value;
			auto scaled = GetScaledParameter(param);

            for (auto& e: channels)
                e->SetParameter(param, scaled);

		}

        void SetScaledParameter(Parameter param, double scaled)
        {
            parameters[(int)param] = GetNormalizedValue(param,scaled);

            for (auto& e: channels)
                e->SetParameter(param, scaled);
        }

		void ClearBuffers()
		{
            for (auto& e: channels)
                e->ClearBuffers();
		}

		void Process(double** input, double** output, int bufferSize)
		{
			auto len = bufferSize;
			auto cm = GetScaledParameter(Parameter::InputMix) * 0.5;
			auto cmi = (1 - cm);

            for (int ch=0;ch<channels.size();ch++)
			for (int i = 0; i < len; i++)
			{
                auto ch2 = (ch+1) % (channels.size());
                channelIns[ch][i] = input[ch][i] * cmi + input[ch2][i] * cm;
			}

            auto idx = 0;
            for (auto& e: channels)
            {
                e->Process(channelIns[idx], len);
                idx++;
            }

            for (int ch=0;ch<channels.size();ch++)
			for (int i = 0; i < len; i++)
			{
                output[ch][i] = channels[ch]->GetOutput()[i];
			}
		}
		
	private:
		double P(Parameter para)
		{
            auto idx = (int)para;
            return ((idx >= 0) && (idx < (int)Parameter::Count)) ? parameters[idx] : 0.0;
		}
	};
}
#endif
