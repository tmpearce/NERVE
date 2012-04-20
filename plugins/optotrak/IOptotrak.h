#pragma once
#include "nrvApp/NerveAPI.h"
#include <string>

#ifdef IOPTOTRAK_EXPORT
#define IOPTOTRAK __declspec(dllexport)
#else
#define IOPTOTRAK __declspec(dllimport)
#endif

struct IOPTOTRAK OptotrakParams
{
	float fFrameFrequency;
	float fMarkerFrequency;
	int nThreshold;
	int nMinimumGain;
	int nStreamData;
	float fDutyCycle;
	float fVoltage;
	float fCollectionTime;
	float fPreTriggerTime;
	int nFlags;
	std::string cameraParamFile;
};


class IOptotrak_private;
class IOPTOTRAK IOptotrak : public IPlugin
{
public:
	void initOptotrak(const OptotrakParams&);
private:
	friend class OptotrakPlugin;
	IOptotrak_private* d_;
	IOptotrak();
	IOptotrak(const IOptotrak& other);
	~IOptotrak();
};