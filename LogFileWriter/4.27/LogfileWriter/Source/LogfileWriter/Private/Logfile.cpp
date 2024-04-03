// Fill out your copyright notice in the Description page of Project Settings.


#include "Logfile.h"

// Sets default values
ALogfile::ALogfile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALogfile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALogfile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALogfile::Destroyed()
{
	Super::Destroyed();

	if (IsLogging)
	{
		// Deleting the file handle will close the text file
		delete FileHandle;
	}

	return;
}

void ALogfile::BeginWrite(const FString& Path, const FString& Filename)
{
	if (IsLogging)
	{
		return;
	}

	Player = GetWorld()->GetFirstPlayerController();

	if (!Player)
	{
		return;
	}

	if (!FPaths::DirectoryExists(FPaths::ProjectDir() + Path))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find directory for logfile"));
		return;
	}
		
	LogfilePath = FPaths::ProjectDir() + Path + "/" + Filename + "_" +
		FDateTime::Now().ToString() + ".txt";
	
	// Open the text file for writing
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();
	FileHandle = File.OpenWrite(*LogfilePath, true);

	UE_LOG(LogTemp, Log, TEXT("Write logfile to %s"), *LogfilePath);

	// Write first row with column names to file
	FString Header = "time\tx\ty\tz\tx_roll\ty_pitch\tz_yaw\tmarker\r\n";
	FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Header), Header.Len());

	MarkerHandler = GetMarkerHandler();

	if (!MarkerHandler)
	{
		UE_LOG(LogTemp, Warning, TEXT("LogfileWriter: Can't find Marker Handler"));
	}

	// Setup a timer for repeatedly calling WriteLogFileEntry()
	GetWorld()->GetTimerManager().SetTimer(LogFileTimerHandle, this, &ALogfile::WriteLogFileEntry, 1.f / 20, true);

	IsLogging = true;

}

void ALogfile::EndWrite()
{
	if (!IsLogging)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(LogFileTimerHandle);

	// Deleting the file handle will close the text file
	delete FileHandle;

	IsLogging = false;
}

AMarkerHandler* ALogfile::GetMarkerHandler()
{
	AMarkerHandler* TempMarkerHandle = nullptr;

	// If this fails, return first instance of AMarkerHandler
	for (TActorIterator<AMarkerHandler> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		TempMarkerHandle = *ActorItr;
		break;
	}

	if (!TempMarkerHandle)
	{
		return nullptr;
	}

	return TempMarkerHandle;
}

int32 ALogfile::GetMarker() const
{
	if (!MarkerHandler)
	{
		return 0;
	}
	else
	{
		return MarkerHandler->GetCurrentMarker();
	}
}

void ALogfile::WriteLogFileEntry()
{
	// Get current position and rotation
	Player->GetPlayerViewPoint(PlayerPosition, PlayerRotation);

	FString Data = FString::Printf(TEXT("%.04f\t%.03f\t%.03f\t%.03f\t%.03f\t%.03f\t%.03f\t%i\r\n"),
		GetWorld()->GetTimeSeconds(),
		PlayerPosition.X, PlayerPosition.Y, PlayerPosition.Z,
		PlayerRotation.Roll, PlayerRotation.Pitch, PlayerRotation.Yaw,
		GetMarker());

	FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Data), Data.Len());
}

