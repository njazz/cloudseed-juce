#ifndef MODULATEDALLPASS
#define MODULATEDALLPASS

#include "ModulatedAllpass.h"
#include "FastSin.h"
#include "Utils.h"

#undef NDEBUG
#include <assert.h>

#if _MSC_VER
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __attribute__((always_inline))
#endif

namespace CloudSeed
{
	class ModulatedAllpass
	{
	public:
        const size_t DelayBufferSamples = 19200; // 100ms at 192Khz
		static const int ModulationUpdateRate = 8;

	private:
        double* delayBuffer = nullptr;
        double* output = nullptr;
        int bufferSize = 4096;
        size_t index = 0; ///< changed
        size_t samplesProcessed = 0; ///< changed unsigned int

        double modPhase = 0;
        size_t delayA = 0; ///< changed
        size_t delayB = 0; ///< changed
        double gainA = 0;
        double gainB = 0;

	public:

        size_t SampleDelay = 0; ///< changed
        double Feedback = 0;
        double ModAmount = 0;
        double ModRate = 0;

        bool InterpolationEnabled = false;
        bool ModulationEnabled = false;

        ModulatedAllpass(int bufferSize, size_t sampleDelay)
		{
            assert(sampleDelay<DelayBufferSamples);

            this->InterpolationEnabled = true;
			this->bufferSize = bufferSize;
			delayBuffer = new double[DelayBufferSamples];
			output = new double[bufferSize];
			SampleDelay = sampleDelay;
			index = DelayBufferSamples - 1;
			modPhase = 0.01 + 0.98 * std::rand() / (double)RAND_MAX;
			ModRate = 0.0;
			ModAmount = 0.0;
			Update();

            // added:
            for (int i=0;i<bufferSize;i++){
                delayBuffer[i]=0;
                output[i]=0;
            }
		}

        ModulatedAllpass(const ModulatedAllpass&) = delete;
        ModulatedAllpass& operator=(const ModulatedAllpass&) = delete;

		~ModulatedAllpass()
		{
			delete delayBuffer;
			delete output;
		}


        FORCE_INLINE double* GetOutput()
		{
			return output;
		}

		void ClearBuffers()
		{
			Utils::ZeroBuffer(delayBuffer, DelayBufferSamples);
			Utils::ZeroBuffer(output, bufferSize);
		}

		void Process(double* input, int sampleCount)
		{
			if (ModulationEnabled)
				ProcessWithMod(input, sampleCount);
			else
				ProcessNoMod(input, sampleCount);
		}


	private:
		void ProcessNoMod(double* input, int sampleCount)
		{
            assert(SampleDelay<DelayBufferSamples);

            long delayedIndex = index - SampleDelay;
			if (delayedIndex < 0) delayedIndex += DelayBufferSamples;

			for (int i = 0; i < sampleCount; i++)
			{
				auto bufOut = delayBuffer[delayedIndex];
				auto inVal = input[i] + bufOut * Feedback;

				delayBuffer[index] = inVal;
				output[i] = bufOut - inVal * Feedback;

				index++;
				delayedIndex++;
				if (index >= DelayBufferSamples) index -= DelayBufferSamples;
				if (delayedIndex >= DelayBufferSamples) delayedIndex -= DelayBufferSamples;
				samplesProcessed++;
			}
		}

		void ProcessWithMod(double* input, int sampleCount)
		{
			for (int i = 0; i < sampleCount; i++)
			{
				if (samplesProcessed >= ModulationUpdateRate)
					Update();

				double bufOut;

				if (InterpolationEnabled)
				{
					int idxA = index - delayA;
					int idxB = index - delayB;
					idxA += DelayBufferSamples * (idxA < 0); // modulo
					idxB += DelayBufferSamples * (idxB < 0); // modulo

					bufOut = delayBuffer[idxA] * gainA + delayBuffer[idxB] * gainB;
				}
				else
				{
					int idxA = index - delayA;
					idxA += DelayBufferSamples * (idxA < 0); // modulo
					bufOut = delayBuffer[idxA];
				}

				auto inVal = input[i] + bufOut * Feedback;
				delayBuffer[index] = inVal;
				output[i] = bufOut - inVal * Feedback;

				index++;
				if (index >= DelayBufferSamples) index -= DelayBufferSamples;
				samplesProcessed++;
			}
		}

        FORCE_INLINE double Get(int delay)
		{
			int idx = index - delay;
			if (idx < 0)
				idx += DelayBufferSamples;

			return delayBuffer[idx];
		}

		void Update()
		{
			modPhase += ModRate * ModulationUpdateRate;
			if (modPhase > 1)
				modPhase = std::fmod(modPhase, 1.0);

			auto mod = FastSin::Get(modPhase);

			if (ModAmount >= SampleDelay) // don't modulate to negative value
				ModAmount = SampleDelay - 1;


			auto totalDelay = SampleDelay + ModAmount * mod;
			
			if (totalDelay <= 0) // should no longer be required
				totalDelay = 1;

			delayA = (int)totalDelay;
			delayB = (int)totalDelay + 1;

			auto partial = totalDelay - delayA;

			gainA = 1 - partial;
			gainB = partial;

			samplesProcessed = 0;
		}

	};
}

#endif
