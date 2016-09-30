// Copyright (c) 2016 Daniel Gromer
// Distributed under the MIT License (license terms are at http ://opensource.org/licenses/MIT)

#include "anxpres_02.h"
#include "LogfileWriter.h"


// Sets default values
ALogfileWriter::ALogfileWriter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsLogging = false;

}

// Called when the game starts or when spawned
void ALogfileWriter::BeginPlay()
{
	Super::BeginPlay();

	SetFileName("data_anxpres3_" + FDateTime::Now().ToString() + ".txt");
}

// Called every frame
void ALogfileWriter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

APawn* ALogfileWriter::GetPlayerPawn()
{
	// Try to find pawn of first player
	APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!Pawn)
	{
		// If this fails, walk through all instances of APawn (this should be only one in this game)
		for (TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			Pawn = *ActorItr;
			break;
		}
	}

	return Pawn;
}

AMarkerHandler * ALogfileWriter::GetMarkerHandler()
{
	AMarkerHandler* TempMarkerHandle = nullptr;

	// If this fails, walk through all instances of APawn (this should be only one in this game)
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

int32 ALogfileWriter::GetMarker() const
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

void ALogfileWriter::SetDirectory(FString NewDirectory)
{
	Directory = NewDirectory;
}

void ALogfileWriter::SetFileName(FString NewFileName)
{
	FileName = NewFileName;
}

void ALogfileWriter::BeginWrite()
{
	if (IsLogging)
	{
		return;
	}

	PlayerPawn = GetPlayerPawn();

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Player Pawn"));
		return;
	}

	MarkerHandler = GetMarkerHandler();

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Marker Handler"));
	}

	FString Path = Directory + "/" + FileName;

	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();
	FileHandle = File.OpenWrite(*Path, true);

	FString Header = "time\tx\ty\tz\tx_roll\ty_pitch\tz_yaw\tmarker\r\n";

	FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Header), Header.Len());

	GetWorld()->GetTimerManager().SetTimer(LogFileTimerHandle, this, &ALogfileWriter::WriteLogFileEntry, 1.f / 20, true);

	IsLogging = true;
}

void ALogfileWriter::WriteLogFileEntry()
{
	FVector CurrentPosition = PlayerPawn->GetActorLocation();
	FRotator CurrentRotation = PlayerPawn->GetActorRotation();

	FString Data = FString::Printf(TEXT("%.04f\t%.03f\t%.03f\t%.03f\t%.03f\t%.03f\t%.03f\t%i\r\n"),
		GetWorld()->GetTimeSeconds(),
		CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z,
		CurrentRotation.Roll, CurrentRotation.Pitch, CurrentRotation.Yaw,
		GetMarker());

	FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Data), Data.Len());
}

void ALogfileWriter::EndWrite()
{
	if (!IsLogging)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(LogFileTimerHandle);
	delete FileHandle;

	IsLogging = false;
}

void ALogfileWriter::MyDebug()
{
	for (TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Warning, TEXT("The pawn:"));
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		APawn *Mesh = *ActorItr;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorItr->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorItr->GetActorLocation().ToString());
	}
}
