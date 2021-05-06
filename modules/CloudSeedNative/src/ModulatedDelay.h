#ifndef MODULATEDDELAY
#define MODULATEDDELAY

#include "ModulatedDelay.h"
#include "Utils.h"
#include "FastSin.h"

#undef NDEBUG
#include <assert.h>

namespace CloudSeed
{
	class ModulatedDelay
	{
	private:

		const int ModulationUpdateRate = 8;

        double* delayBuffer = nullptr;
        double* output = nullptr;
        int bufferSize = 4096;
        size_t writeIndex = 0;
        long readIndexA = 0;
        long readIndexB = 0;
        int samplesProcessed = 0;
        int delayBufferSizeSamples = 44100;

        double modPhase = 0;
        double gainA = 0;
        double gainB = 0;

	public:
        int SampleDelay = 0;
		
        double ModAmount = 0;
        double ModRate = 0;

        ModulatedDelay(size_t bufferSize, int delayBufferSizeSamples, size_t sampleDelay)
		{
            assert(sampleDelay<delayBufferSizeSamples);

			this->delayBufferSizeSamples = delayBufferSizeSamples;
			this->bufferSize = bufferSize;
			this->delayBuffer = new double[delayBufferSizeSamples];
			this->output = new double[bufferSize];
			this->SampleDelay = sampleDelay;
			writeIndex = 0;
			modPhase = 0.01 + 0.98 * (std::rand() / (double)RAND_MAX);
			ModRate = 0.0;
			ModAmount = 0.0;
			Update();

            // added:
            for (int i=0;i<delayBufferSizeSamples;i++){
                delayBuffer[i]=0;
            }
            for (int i=0;i<bufferSize;i++){
                output[i]=0;
            }
		}

        ModulatedDelay(const ModulatedDelay&) = delete;
        ModulatedDelay operator=(const ModulatedDelay&) = delete;

		~ModulatedDelay()
		{
			delete delayBuffer;
			delete output;
		}

		double* GetOutput()
		{
			return output;
		}

		void Process(double* input, int sampleCount)
		{
			for (int i = 0; i < sampleCount; i++)
			{
                // new
                assert(readIndexA >= 0);
                assert(readIndexB >= 0);
                assert(writeIndex < delayBufferSizeSamples);

				if (samplesProcessed == ModulationUpdateRate)
					Update();

				delayBuffer[writeIndex] = input[i];
				output[i] = delayBuffer[readIndexA] * gainA + delayBuffer[readIndexB] * gainB;

				writeIndex++;
				readIndexA++;
				readIndexB++;
				if (writeIndex >= delayBufferSizeSamples) writeIndex -= delayBufferSizeSamples;
				if (readIndexA >= delayBufferSizeSamples) readIndexA -= delayBufferSizeSamples;
				if (readIndexB >= delayBufferSizeSamples) readIndexB -= delayBufferSizeSamples;
				samplesProcessed++;


			}
		}

		void ClearBuffers()
		{
			Utils::ZeroBuffer(delayBuffer, delayBufferSizeSamples);
			Utils::ZeroBuffer(output, bufferSize);
		}


	private:
		void Update()
		{
			modPhase += ModRate * ModulationUpdateRate;
			if (modPhase > 1)
                modPhase = std::fmod(modPhase, 1.0);

            // added:
            if (modPhase < 0)
                modPhase = 1 - std::fmod(-modPhase, 1.0);
            assert(modPhase>0);
            assert(modPhase<1);

			auto mod = FastSin::Get(modPhase);
			auto totalDelay = SampleDelay + ModAmount * mod;

//            assert(abs(mod)<1);
            assert(totalDelay<delayBufferSizeSamples);

			auto delayA = (int)totalDelay;
			auto delayB = (int)totalDelay + 1;

			auto partial = totalDelay - delayA;

			gainA = 1 - partial;
			gainB = partial;

			readIndexA = writeIndex - delayA;
			readIndexB = writeIndex - delayB;
			if (readIndexA < 0) readIndexA += delayBufferSizeSamples;
			if (readIndexB < 0) readIndexB += delayBufferSizeSamples;

			samplesProcessed = 0;

            // new
            assert(readIndexA>=0);
            assert(readIndexB>=0);

		}
	};
}

#endif
