// Copyright (c) 2018 Daniel Gromer
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT)

#include "MarkerHandler.h"


// Sets default values
AMarkerHandler::AMarkerHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InpOutLibLoaded = false;
}

// Called when the game starts or when spawned
void AMarkerHandler::BeginPlay()
{
	Super::BeginPlay();
	
	BindInpOutDLL();
}

// Called every frame
void AMarkerHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMarkerHandler::Destroyed()
{
	if (InpOutLibLoaded)
	{
		FPlatformProcess::FreeDllHandle(DLLHandle);
	}
}

int32 AMarkerHandler::GetCurrentMarker()
{
	if (MarkerQueue.IsEmpty())
	{
		CurrentMarker = 0;
	}
	else
	{
		MarkerQueue.Dequeue(CurrentMarker);

		WriteMarkerLPT((int16)CurrentMarker);
	}

	return CurrentMarker;
}

void AMarkerHandler::SetMarker(int32 Marker)
{
	MarkerQueue.Enqueue(Marker);
}

void AMarkerHandler::BindInpOutDLL()
{
	FString FilePath = FPaths::Combine(*FPaths::ProjectPluginsDir(), TEXT("InpOutx64/"), TEXT("inpoutx64.dll"));

	if (FPaths::FileExists(FilePath))
	{
		// Retrieve the DLL
		DLLHandle = FPlatformProcess::GetDllHandle(*FilePath);

		if (DLLHandle != NULL)
		{
			// The exact name of the DLL function
			FString ProcName = "Out32";

			// Export the DLL function.
			_lpOut32 = (lpOut32)FPlatformProcess::GetDllExport(DLLHandle, *ProcName);

			InpOutLibLoaded = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not load inpoutx64.dll. Make sure it is located in YOURGAME/Plugins/InpOutx64/"));

		InpOutLibLoaded = false;
	}
}

void AMarkerHandler::WriteMarkerLPT(int16 Marker)
{
	if (InpOutLibLoaded)
	{
		_lpOut32((uint32)0xD010, Marker);

		// Create a timer that resets the port output after 50 ms.
		GetWorld()->GetTimerManager().SetTimer(MarkerHandlerTimerHandle, this, &AMarkerHandler::ResetLPT, 1.f / 20, false);
	}

}

void AMarkerHandler::ResetLPT()
{
	_lpOut32((uint32)0xD010, 0);
}
