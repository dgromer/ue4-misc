// Copyright (c) 2016 Daniel Gromer
// Distributed under the MIT License (license terms are at http ://opensource.org/licenses/MIT)

#include "anxpres_02.h"
#include "MarkerHandler.h"


// Sets default values
AMarkerHandler::AMarkerHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMarkerHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMarkerHandler::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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
	}

	return CurrentMarker;
}

void AMarkerHandler::SetMarker(int32 Marker)
{
	MarkerQueue.Enqueue(Marker);
}
