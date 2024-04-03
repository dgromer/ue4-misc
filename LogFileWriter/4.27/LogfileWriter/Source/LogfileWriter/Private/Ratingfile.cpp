// Fill out your copyright notice in the Description page of Project Settings.


#include "Ratingfile.h"

// Sets default values
ARatingfile::ARatingfile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARatingfile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARatingfile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARatingfile::BeginWrite(const FString& Path, const FString& Filename)
{
	if (IsLogging)
	{
		return;
	}

	if (!FPaths::DirectoryExists(FPaths::ProjectDir() + Path))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find directory for ratingfile"));
		return;
	}

	RatingfilePath = FPaths::ProjectDir() + Path + "/" + Filename + "_" +
		FDateTime::Now().ToString() + ".txt";

	// Open the text file for writing
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();
	FileHandle = File.OpenWrite(*RatingfilePath, true);

	UE_LOG(LogTemp, Log, TEXT("Write ratingfile to %s"), *RatingfilePath);

	// Write first row with column names to file
	FString Header = "phase\tcsz\trating\tvalue\r\n";
	FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Header), Header.Len());

	IsLogging = true;

}

void ARatingfile::EndWrite()
{
	if (!IsLogging)
	{
		return;
	}

	// Deleting the file handle will close the text file
	delete FileHandle;

	IsLogging = false;

}

void ARatingfile::WriteRatingFileEntry(const FString& Phase, const int32 CSz, const FString& Rating, const int32 Value)
{
	if (IsLogging)
	{
		FString Data = FString::Printf(TEXT("%s\t%i\t%s\t%i\r\n"),
			*Phase,
			CSz,
			*Rating,
			Value);

		FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Data), Data.Len());
	}
}

