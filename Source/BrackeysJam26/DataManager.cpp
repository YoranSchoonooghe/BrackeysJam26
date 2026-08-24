// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Math/UnrealMathUtility.h"

ADataManager::ADataManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADataManager::BeginPlay()
{
	Super::BeginPlay();

	//LoadTextFileIntoArray(TEXT("FirstNames.txt"), FirstNames);
	//LoadTextFileIntoArray(TEXT("LastNames.txt"), LastNames);
	//LoadTextFileIntoArray(TEXT("IDNumbers.txt"), IDNumbers);
	//LoadTextFileIntoArray(TEXT("Species.txt"), SpeciesList);
	LoadTextFileIntoArray(TEXT("BusStops.txt"), BusStopNames);

	GenerateActiveRoute();
}

void ADataManager::LoadTextFileIntoArray(FString FileName, TArray<FString>& OutArray)
{
	FString FilePath = FPaths::ProjectContentDir() + TEXT("Data/") + FileName;
	FFileHelper::LoadFileToStringArray(OutArray, *FilePath);
}

FPassengerData ADataManager::GenerateRandomPassenger()
{
	FPassengerData NewPassenger;

	if (FirstNames.Num() > 0)
	{
		NewPassenger.FirstName = FirstNames[FMath::RandRange(0, FirstNames.Num() - 1)];
	}
	if (LastNames.Num() > 0)
	{
		NewPassenger.LastName = LastNames[FMath::RandRange(0, LastNames.Num() - 1)];
	}
	if (IDNumbers.Num() > 0)
	{
		NewPassenger.IDNumber = IDNumbers[FMath::RandRange(0, IDNumbers.Num() - 1)];
	}
	if (SpeciesList.Num() > 0)
	{
		NewPassenger.Species = SpeciesList[FMath::RandRange(0, SpeciesList.Num() - 1)];
	}

	return NewPassenger;
}

FPassengerData ADataManager::GenerateImposterPassport(FPassengerData TrueIdentity)
{
	FPassengerData ForgedPassport = TrueIdentity;

	int DiscrepancyType = FMath::RandRange(1, 3);

	switch (DiscrepancyType)
	{
	case 1:
		//Typo
		ForgedPassport.FirstName = GenerateTypo(TrueIdentity.FirstName);
		break;

	case 2:
		//Different Name
		if (FirstNames.Num() > 0)
		{
			ForgedPassport.FirstName = FirstNames[FMath::RandRange(0, FirstNames.Num() - 1)];
		}
		break;

	case 3:
		//Fake ID Number
		ForgedPassport.IDNumber = GenerateTypo(TrueIdentity.IDNumber);
		break;
	}

	return ForgedPassport;
}

FString ADataManager::GenerateTypo(FString OriginalText)
{
	//Swap character with the one next to it
	if (OriginalText.Len() <= 3) return OriginalText;

	int SwapIndex = FMath::RandRange(1, OriginalText.Len() - 2);

	TCHAR Temp = OriginalText[SwapIndex];
	OriginalText[SwapIndex] = OriginalText[SwapIndex + 1];
	OriginalText[SwapIndex + 1] = Temp;

	return OriginalText;
}

FString ADataManager::GetStopNameByIndex(int Index)
{
	if (ActiveRoute.IsValidIndex(Index))
	{
		return TEXT("Next stop: ") + ActiveRoute[Index];
	}
	return TEXT("Final Destination");
}

void ADataManager::GenerateActiveRoute()
{
	ActiveRoute.Empty();

	TArray<FString> ShuffledStops = BusStopNames;

	for (int i = ShuffledStops.Num() - 1; i > 0; i--)
	{
		int SwapIndex = FMath::RandRange(0, i);
		ShuffledStops.Swap(i, SwapIndex);
	}

	int StopsToPick = FMath::Min(NumberOfStops, ShuffledStops.Num());

	for (int i = 0; i < StopsToPick; i++)
	{
		ActiveRoute.Add(ShuffledStops[i]);
	}
}

TArray<FPassengerRecord> ADataManager::GeneratePassengerQueue(int TotalPassengers, int MinImposters, int MaxImposters)
{
	TArray<FPassengerRecord> DailyQueue;

	int TargetImposters = FMath::RandRange(MinImposters, MaxImposters);
	int CurrentImposters = 0;

	for (int i = 0; i < TotalPassengers; i++)
	{
		FPassengerRecord NewRecord;
		NewRecord.TrueIdentity = GenerateRandomPassenger();

		if (CurrentImposters < TargetImposters)
		{
			NewRecord.bIsImposter = true;
			NewRecord.PresentedPassport = GenerateImposterPassport(NewRecord.TrueIdentity);
			CurrentImposters++;
		}
		else
		{
			NewRecord.bIsImposter = false;
			NewRecord.PresentedPassport = NewRecord.TrueIdentity;
		}

		DailyQueue.Add(NewRecord);
	}

	for (int i = DailyQueue.Num() - 1; i > 0; i--)
	{
		int SwapIndex = FMath::RandRange(0, i);
		DailyQueue.Swap(i, SwapIndex);
	}

	return DailyQueue;
}
