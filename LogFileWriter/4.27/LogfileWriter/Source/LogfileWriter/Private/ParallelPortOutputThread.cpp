// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallelPortOutputThread.h"

// Sets default values
FParallelPortOutputThread::FParallelPortOutputThread(AMarkerHandler* MarkerHandler, FString PortAddress)
{
    CurrentMarkerHandler = MarkerHandler;
    CurrentPortAddress = PortAddress;

    InpOutLibLoaded = false;
}

bool FParallelPortOutputThread::Init()
{
    bStopThread = false;

    BindInpOutDLL();

    return true;
}

uint32 FParallelPortOutputThread::Run()
{
    std::string PortAsStdString(TCHAR_TO_UTF8(*CurrentPortAddress));
    unsigned long PortAsLong = std::stoul(PortAsStdString, nullptr, 16);

    while (!bStopThread)
    {
        if (!CurrentMarkerHandler->ThreadMarkerQueue.IsEmpty() && CurrentMarkerHandler->ThreadMarkerQueue.Dequeue(Intensity))
        {            
            // Number of shocks in the current train
            TArray<int32> ShockIntensities = { 0, 0, 1, 3, 6, 10, 16, 24 };
                        
            for (int i = 0; i < ShockIntensities[Intensity - 1]; i++)
            {
                _lpOut32((uint32)PortAsLong, 128);
                // _lpOut32((uint32)PortAsLong, 1);

                FPlatformProcess::Sleep(0.002);

                _lpOut32((uint32)PortAsLong, 0);

                FPlatformProcess::Sleep(0.006);
                
            }
        }
        else
        {
            FPlatformProcess::Sleep(0.001);
        }
    }

    return 0;
}

void FParallelPortOutputThread::Stop()
{
    if (InpOutLibLoaded)
    {
        ResetLPT();
        FPlatformProcess::FreeDllHandle(DLLHandle);
    }
}

void FParallelPortOutputThread::BindInpOutDLL()
{
    FString FilePath = FPaths::Combine(*FPaths::ProjectPluginsDir(), TEXT("LogfileWriter/Binaries/Thirdparty/inpoutx64/inpoutx64.dll"));

    if (FPaths::FileExists(FilePath))
    {
        // Retrieve the DLL
        DLLHandle = FPlatformProcess::GetDllHandle(*FilePath);

        if (DLLHandle != NULL)
        {
            // The exact name of the DLL function
            FString ProcName = "Out32";

            // Export the DLL function.
            _lpOut32 = (lpOut32)FPlatformProcess::GetDllExport(DLLHandle, *ProcName);

            InpOutLibLoaded = true;
        }
    }
}

void FParallelPortOutputThread::ResetLPT()
{
    std::string PortAsStdString(TCHAR_TO_UTF8(*CurrentPortAddress));
    unsigned long PortAsLong = std::stoul(PortAsStdString, nullptr, 16);

    _lpOut32((uint32)PortAsLong, 0);
}
