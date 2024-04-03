// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Public/HAL/Runnable.h"
#include "MarkerHandler.h"
#include <string>

class FRunnableThread;
class AMarkerHandler;

/**
 * 
 */
class LOGFILEWRITER_API FParallelPortOutputThread : public FRunnable
{

public:
	// Sets default values for this actor's properties
	FParallelPortOutputThread(AMarkerHandler* MarkerHandler, FString PortAddress);

	bool bStopThread;

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

private:

	int32 Intensity;

	AMarkerHandler* CurrentMarkerHandler;
	FString CurrentPortAddress;

	bool InpOutLibLoaded;
	void* DLLHandle;
	// Declare DLL function and function pointer
	typedef void(__stdcall* lpOut32)(short, short);
	lpOut32 _lpOut32;

	void BindInpOutDLL();
	void ResetLPT();

};

