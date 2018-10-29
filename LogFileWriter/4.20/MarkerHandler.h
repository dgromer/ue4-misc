// Copyright (c) 2018 Daniel Gromer
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "MarkerHandler.generated.h"

UCLASS()
class OFT_2_API AMarkerHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarkerHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable, Category = "MarkerHandler")
		int32 GetCurrentMarker();

	UFUNCTION(BlueprintCallable, Category = "MarkerHandler")
		void SetMarker(int32 Marker);

	FTimerHandle MarkerHandlerTimerHandle;

private:

	int32 CurrentMarker;
	TQueue<int32> MarkerQueue;

	bool InpOutLibLoaded;
	void *DLLHandle;
	// Declare DLL function and function pointer
	typedef void(__stdcall *lpOut32)(short, short);
	lpOut32 _lpOut32;

	void BindInpOutDLL();
	void WriteMarkerLPT(int16 Marker);
	void ResetLPT();
	
};
