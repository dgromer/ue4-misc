// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkerHandler.h"
#include <string>

// Sets default values
AMarkerHandler::AMarkerHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InpOutLibLoaded = false;

}

// Function to be called from the LogfileWriter actor to get the current marker and execute the LPT call
int32 AMarkerHandler::GetCurrentMarker()
{
	// If no marker is queued, simply return 0
	if (MarkerQueue.IsEmpty())
	{
		CurrentMarker = 0;
	}
	else
	{
		// Get the marker from the marker queue
		MarkerQueue.Dequeue(CurrentMarker);

		// Only write to the LPT port, if a port address was defined
		if (!PortAddress.IsEmpty())
		{
			WriteMarkerLPT((int16)CurrentMarker);
		}
	}

	return CurrentMarker;

}

void AMarkerHandler::SetMarker(int32 Marker)
{
	MarkerQueue.Enqueue(Marker);

}

void AMarkerHandler::SendShockTrainLPT(int32 Intensity)
{
    if (SupportStimulusTrains && CurrentRunningThread && ParallelportOutputThread)
    {
        ThreadMarkerQueue.Enqueue(Intensity);
    }
}

void AMarkerHandler::WriteMarkerLPT(int32 Marker, const bool AutoReset)
{
	// if (SupportStimulusTrains && Marker == (int32)128 && CurrentRunningThread && ParallelportOutputThread)
	// {
	// 	ThreadMarkerQueue.Enqueue(8);
	// }
	// else if (InpOutLibLoaded)
    if (InpOutLibLoaded)
	{
		std::string PortAsStdString(TCHAR_TO_UTF8(*PortAddress));
		unsigned long PortAsLong = std::stoul(PortAsStdString, nullptr, 16);
		
		_lpOut32((uint32)PortAsLong, Marker);

		if (AutoReset)
		{
			// Create a timer that resets the port output after 10 ms.
			GetWorld()->GetTimerManager().SetTimer(MarkerHandlerTimerHandle, this, &AMarkerHandler::ResetLPT, 0.01, false);
		}	
	}

}

// Called when the game starts or when spawned
void AMarkerHandler::BeginPlay()
{
	Super::BeginPlay();

	BindInpOutDLL();

	if (SupportStimulusTrains)
	{
		InitThread();
	}
	
}

void AMarkerHandler::InitThread()
{
	ParallelportOutputThread = new FParallelPortOutputThread(this, PortAddress);
	CurrentRunningThread = FRunnableThread::Create(ParallelportOutputThread, TEXT("Parallel Port Output Thread"));
}

// Called every frame
void AMarkerHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMarkerHandler::Destroyed()
{
	Super::Destroyed();

	if (InpOutLibLoaded)
	{
		FPlatformProcess::FreeDllHandle(DLLHandle);
	}

}

void AMarkerHandler::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (CurrentRunningThread && ParallelportOutputThread)
	{
		CurrentRunningThread->Suspend(true);
		ParallelportOutputThread->bStopThread = true;
		CurrentRunningThread->Suspend(false);
		CurrentRunningThread->Kill(true);
		CurrentRunningThread->WaitForCompletion();
		delete ParallelportOutputThread;
	}
}

void AMarkerHandler::BindInpOutDLL()
{
	FString FilePath = FPaths::Combine(*FPaths::ProjectPluginsDir(), TEXT("LogfileWriter/Binaries/Thirdparty/inpoutx64/inpoutx64.dll"));

	if (FPaths::FileExists(FilePath))
	{
		// Retrieve the DLL
		DLLHandle = FPlatformProcess::GetDllHandle(*FilePath);

		if (DLLHandle != NULL)
		{
			UE_LOG(LogTemp, Log, TEXT("inpoutx64.dll successfully loaded!"));

			// The exact name of the DLL function
			FString ProcName = "Out32";

			// Export the DLL function.
			_lpOut32 = (lpOut32)FPlatformProcess::GetDllExport(DLLHandle, *ProcName);

			InpOutLibLoaded = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not load inpoutx64.dll. Make sure it is located in YOURGAME/Plugins/LogfileWriter/Binaries/Thirdparty/inpoutx64/"));

		InpOutLibLoaded = false;
	}

}

void AMarkerHandler::ResetLPT()
{
	std::string PortAsStdString(TCHAR_TO_UTF8(*PortAddress));
	unsigned long PortAsLong = std::stoul(PortAsStdString, nullptr, 16);

	_lpOut32((uint32)PortAsLong, 0);

}

