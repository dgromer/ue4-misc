// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ratingfile.generated.h"

UCLASS()
class LOGFILEWRITER_API ARatingfile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARatingfile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Open a file for writing and set a timer
	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
		void BeginWrite(const FString& Path, const FString& Filename);

	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
		void EndWrite();

	// Write a line in the logfile
	UFUNCTION(BlueprintCallable, Category = "LogfileWriter")
	void WriteRatingFileEntry(const FString& Phase, const int32 CSz, const FString& Rating, const int32 Value);

private:
	IFileHandle* FileHandle;

	FString RatingfilePath;
	bool IsLogging;

};
