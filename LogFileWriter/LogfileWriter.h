// Copyright (c) 2016 Daniel Gromer
// Distributed under the MIT License (license terms are at http ://opensource.org/licenses/MIT)

#pragma once

#include "GameFramework/Actor.h"
#include "MarkerHandler.h"
#include "LogfileWriter.generated.h"

UCLASS()
class ANXPRES_02_API ALogfileWriter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALogfileWriter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
	void SetDirectory(FString NewDirectory);

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
	void SetFileName(FString NewFileName);

	// Open a file for writing and set a timer
	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
	void BeginWrite();

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
	void EndWrite();

	FTimerHandle LogFileTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
	void MyDebug();
	
private:

	IFileHandle* FileHandle;

	FString Directory;
	FString FileName;
	bool IsLogging;

	APawn* PlayerPawn;
	AMarkerHandler* MarkerHandler;

	// Returns a pointer to the pawn of the first player
	APawn* GetPlayerPawn();

	AMarkerHandler* GetMarkerHandler();
	int32 GetMarker() const;

	// Write a line in the logfile
	void WriteLogFileEntry();

};
