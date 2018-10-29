// Copyright (c) 2018 Daniel Gromer
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "MarkerHandler.h"
#include "LogfileWriter.generated.h"

UCLASS()
class OFT_2_API ALogfileWriter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALogfileWriter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
		void SetDirectory(const FString &NewDirectory);

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
		void SetFileName(const FString &NewFileName);

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

	APlayerController* Player;
	FVector PlayerPosition;
	FRotator PlayerRotation;
	AMarkerHandler* MarkerHandler;

	AMarkerHandler* GetMarkerHandler();
	int32 GetMarker() const;

	// Write a line in the logfile
	void WriteLogFileEntry();
	
};
