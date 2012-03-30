#pragma once
#include "ndtypes.h"
#include "ndpack.h"
#include "ndopto.h"
#include <string>
#include <windows.h>

#define BUFFER_SIZE 60000
#define NUM_MARKERS 1
class OptotrakModule
{
public:
	OptotrakModule():paramsSet(false),dataFileOpen(false),mp_DataFile(0){}
	bool initOptotrak()
	{
		if(!paramsSet)
		{
			printf("Failed in OptotrakModule::initOptotrak - parameters not set when init called\n");
			return false;
		}
		int nNumSensors, nNumOdaus, nMarkers;
		char szNDErrorString[MAX_ERROR_STRING_LENGTH + 1];

		if(TransputerDetermineSystemCfg("logfile\0"))
		{
			printf("Failed in OptotrakModule::initOptotrak - TransputerDetermineSystemCfg\n");
			return false;
		}
		if(TransputerLoadSystem("system"))
		{
			printf("Failed in OptotrakModule::initOptotrak - TransputerLoadSystem\n");
			return false;
		}
		Sleep(1000); //wait one second for the system to load
		if(TransputerInitializeSystem(OPTO_LOG_ERRORS_FLAG))
		{
			printf("Failed in OptotrakModule::initOptotrak - TransputerInitialize\n");
			return false;
		}
		if(OptotrakLoadCameraParameters(const_cast<char*>( m_cameraParamFile.c_str() )))
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakLoadCameraParameters\n");
			return false;
		}

		//Request and receive the OPTOTRAK status.
		if(OptotrakGetStatus(
				&nNumSensors,    /* Number of sensors in the OPTOTRAK system. */
				&nNumOdaus,      /* Number of ODAUs in the OPTOTRAK system. */
				NULL,            /* Number of rigid bodies being tracked by the O/T. */
				&nMarkers,       /* Number of markers in the collection. */
				NULL,            /* Frequency that data frames are being collected. */
				NULL,            /* Marker firing frequency. */
				NULL,            /* Dynamic or Static Threshold value being used. */
				NULL,            /* Minimum gain code amplification being used. */
				NULL,            /* Stream mode indication for the data buffers */
				NULL,            /* Marker Duty Cycle being used. */
				NULL,            /* Voltage being used when turning on markers. */
				NULL,            /* Number of seconds data is being collected. */
				NULL,            /* Number of seconds data is being pre-triggered. */
				NULL ))         /* Configuration flags. */
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakGetStatus\n");
		}
		
	    
		if(OptotrakSetupCollection(
				NUM_MARKERS,    /* Number of markers in the collection. */
				(float)100.0,   /* Frequency to collect data frames at. */
				(float)2500.0,  /* Marker frequency for marker maximum on-time. */
				30,             /* Dynamic or Static Threshold value to use. */
				 160,            /* Minimum gain code amplification to use. */
				1,              /* Stream mode for the data buffers. */
				(float)0.35,    /* Marker Duty Cycle to use. */
				(float)7.0,     /* Voltage to use when turning on markers. */
				(float)20.0,     /* Number of seconds of data to collect. */
				(float)0.0,     /* Number of seconds to pre-trigger data by. */
				0))/*OPTOTRAK_BUFFER_RAW_FLAG | OPTOTRAK_GET_NEXT_FRAME_FLAG*/ 
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakSetupCollection\n");
			return false;
		}
				
		Sleep(1000);
		
		if(OptotrakActivateMarkers())
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakActivateMarkers\n");
			return false;
		}
		return true;
	}
	void setOptotrakParameters( float fFrameFrequency,
								float fMarkerFrequency,
								int nThreshold,
								int nMinimumGain,
								int nStreamData,
								float fDutyCycle,
								float fVoltage,
								float fCollectionTime,
								float fPreTriggerTime,
								int nFlags,
								std::string cameraParamFile)
	{
		m_nMarkers=NUM_MARKERS;
		m_fFrameFrequency=fFrameFrequency;
		m_fMarkerFrequency=fMarkerFrequency;
		m_nThreshold=nThreshold;
		m_nMinimumGain=nMinimumGain;
		m_nStreamData=nStreamData;
		m_fDutyCycle=fDutyCycle;
		m_fCollectionTime=fCollectionTime;
		m_fPreTriggerTime=fPreTriggerTime;
		m_nFlags=nFlags;
		m_cameraParamFile=cameraParamFile;

		if(fCollectionTime > (m_fFrameFrequency*m_nMarkers*3))
		{
			printf("WARNING: collection time, frame rate and number "
				   "of markers make buffer overruns possible");
		}
		
		paramsSet = true;
	}
	bool setFile(std::string file_with_path)
	{
		m_datafile=file_with_path;
		mp_DataFile = fopen(m_datafile.c_str(),"a");
		if(mp_DataFile==0)
		{
			printf("Error in OptotrakModule::setFile - file '%s' not opened\n",m_datafile.c_str());
			return false;
		}
		dataFileOpen=true;
		return true;
	}
private:
	float m_opto_buffer[BUFFER_SIZE];
	std::string m_cameraParamFile;
	std::string m_datafile;
	
	bool paramsSet;
	bool dataFileOpen;

	FILE* mp_DataFile;

	int m_nMarkers;
	float m_fFrameFrequency;
	float m_fMarkerFrequency;
	int m_nThreshold;
	int m_nMinimumGain;
	int m_nStreamData;
	float m_fDutyCycle;
	float m_fVoltage;
	float m_fCollectionTime;
	float m_fPreTriggerTime;
	int m_nFlags;
	
};