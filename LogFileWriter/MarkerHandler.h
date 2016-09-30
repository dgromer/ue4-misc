// Copyright (c) 2016 Daniel Gromer
// Distributed under the MIT License (license terms are at http ://opensource.org/licenses/MIT)

#pragma once

#include "GameFramework/Actor.h"
#include "MarkerHandler.generated.h"

UCLASS()
class ANXPRES_02_API AMarkerHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarkerHandler();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "MarkerHandler")
	int32 GetCurrentMarker();

	UFUNCTION(BlueprintCallable, Category = "MarkerHandler")
	void SetMarker(int32 Marker);

private:

	int32 CurrentMarker;

	TQueue<int32> MarkerQueue;
	
};
