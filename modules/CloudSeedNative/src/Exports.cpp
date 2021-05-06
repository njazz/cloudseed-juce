
#include "Default.h"
#include "ReverbController.h"
#include "FastSin.h"
#include "AudioLib/ValueTables.h"

#ifdef _MSC_VER
#define DLLEXPORT __dllexport
#else
#define DLLEXPORT
#endif

using namespace CloudSeed;
bool isInitialized = false;

extern "C"
{
    DLLEXPORT ReverbController* Create(int samplerate)
	{
		if (!isInitialized)
		{
			AudioLib::ValueTables::Init();
			FastSin::Init();
			isInitialized = true;
		}

		return new ReverbController(samplerate);
	}

    DLLEXPORT void Delete(ReverbController* item)
	{
		delete item;
	}

    DLLEXPORT int GetSamplerate(ReverbController* item)
	{
		return item->GetSamplerate();
	}

    DLLEXPORT void SetSamplerate(ReverbController* item, int samplerate)
	{
		return item->SetSamplerate(samplerate);
	}

    DLLEXPORT int GetParameterCount(ReverbController* item)
	{
		return item->GetParameterCount();
	}

    DLLEXPORT double* GetAllParameters(ReverbController* item)
	{
		return item->GetAllParameters();
	}

    DLLEXPORT double GetScaledParameter(ReverbController* item, Parameter param)
	{
		return item->GetScaledParameter(param);
	}

    DLLEXPORT void SetParameter(ReverbController* item, Parameter param, double value)
	{
		item->SetParameter(param, value);
	}

    DLLEXPORT void ClearBuffers(ReverbController* item)
	{
		item->ClearBuffers();
	}

    DLLEXPORT void Process(ReverbController* item, double** input, double** output, int bufferSize)
	{
		item->Process(input, output, bufferSize);
	}
}
