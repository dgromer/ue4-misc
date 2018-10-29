// Copyright (c) 2018 Daniel Gromer
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT)

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

	SetFileName("data_oft1_" + FDateTime::Now().ToString() + ".txt");
	
}

// Called every frame
void ALogfileWriter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMarkerHandler* ALogfileWriter::GetMarkerHandler()
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

void ALogfileWriter::SetDirectory(const FString &NewDirectory)
{
	Directory = NewDirectory;
}

void ALogfileWriter::SetFileName(const FString &NewFileName)
{
	FileName = NewFileName;
}

void ALogfileWriter::BeginWrite()
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

	MarkerHandler = GetMarkerHandler();

	if (!MarkerHandler)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Marker Handler"));
	}

	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();

	// Open the text file for writing
	FString Path = Directory + "/" + FileName;
	FileHandle = File.OpenWrite(*Path, true);

	// Write first row with column names to file
	FString Header = "time\tx\ty\tz\tx_roll\ty_pitch\tz_yaw\tmarker\r\n";
	FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Header), Header.Len());

	// Setup a timer for repeatedly calling WriteLogFileEntry()
	GetWorld()->GetTimerManager().SetTimer(LogFileTimerHandle, this, &ALogfileWriter::WriteLogFileEntry, 1.f / 20, true);

	IsLogging = true;
}

void ALogfileWriter::WriteLogFileEntry()
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

void ALogfileWriter::EndWrite()
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

void ALogfileWriter::MyDebug()
{
	FVector position;
	FRotator rotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(position, rotation);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *position.ToString());

	//for (TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("The pawn:"));
	//	// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
	//	APawn *Mesh = *ActorItr;
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorItr->GetName());
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorItr->GetActorLocation().ToString());
	//}
}

