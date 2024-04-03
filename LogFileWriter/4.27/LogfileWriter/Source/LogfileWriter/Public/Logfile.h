// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "MarkerHandler.h"
#include "Logfile.generated.h"

UCLASS()
class LOGFILEWRITER_API ALogfile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALogfile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	// Open a file for writing and set a timer
	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
		void BeginWrite(const FString &Path, const FString &Filename);

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
		void EndWrite();

	FTimerHandle LogFileTimerHandle;

private:
	IFileHandle* FileHandle;

	FString LogfilePath;
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
