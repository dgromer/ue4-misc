// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "ParallelPortOutputThread.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "MarkerHandler.generated.h"

UCLASS()
class LOGFILEWRITER_API AMarkerHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarkerHandler();

	int32 GetCurrentMarker();

	UFUNCTION(BlueprintCallable, Category = "MarkerHandler")
		void SetMarker(int32 Marker);

	UFUNCTION(BlueprintCallable, Category = "MarkerHandler")
		void WriteMarkerLPT(int32 Marker, const bool AutoReset = true);

        UFUNCTION(BlueprintCallable, Category = "MarkerHandler")
		void SendShockTrainLPT(int32 Intensity);

	/** Address of the parallel port (e.g., 0x3FF8) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		FString PortAddress;

	/** Whether to support output of stimulus trains (e.g. 2 ms pulses) on the parallel port */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		bool SupportStimulusTrains;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class FParallelPortOutputThread* ParallelportOutputThread = nullptr;

	FRunnableThread* CurrentRunningThread = nullptr;

	void InitThread();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle MarkerHandlerTimerHandle;

	TQueue<int32> ThreadMarkerQueue;

private:
	int32 CurrentMarker;
	TQueue<int32> MarkerQueue;

	bool InpOutLibLoaded;
	void* DLLHandle;
	// Declare DLL function and function pointer
	typedef void(__stdcall* lpOut32)(short, short);
	lpOut32 _lpOut32;

	void BindInpOutDLL();
	void ResetLPT();


};
