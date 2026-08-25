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

	FDateTime Today = FDateTime::Now();
	CurrentGameDate = FString::Printf(TEXT("%04d-%02d-%02d"), Today.GetYear(), Today.GetMonth(), Today.GetDay());


	LoadTextFileIntoArray(TEXT("FirstNames.txt"), FirstNames);
	LoadTextFileIntoArray(TEXT("LastNames.txt"), LastNames);
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
	
	NewPassenger.IDNumber = FString::Printf(TEXT("%06d"), FMath::RandRange(100000, 999999));


	int Year = FMath::RandRange(1950, 2005);
	int Month = FMath::RandRange(1, 12);
	int Day = FMath::RandRange(1, 28);
	NewPassenger.DOB = FString::Printf(TEXT("%04d-%02d-%02d"), Year, Month, Day);

	return NewPassenger;
}

FPassengerData ADataManager::GenerateImposterPassport(FPassengerData TrueIdentity)
{
	FPassengerData ForgedPassport = TrueIdentity;

	int DiscrepancyType = FMath::RandRange(1, 7);

	switch (DiscrepancyType)
	{
	case 1:
		//Typo in First Name
		ForgedPassport.FirstName = GenerateTypo(TrueIdentity.FirstName);
		break;

	case 2:
		//Different First Name
		if (FirstNames.Num() > 0)
		{
			ForgedPassport.FirstName = FirstNames[FMath::RandRange(0, FirstNames.Num() - 1)];
		}
		break;

	case 3:
		//Typo in ID Number
		ForgedPassport.IDNumber = GenerateTypo(TrueIdentity.IDNumber);
		break;

	case 4:
		//Typo in Last Name
		ForgedPassport.LastName = GenerateTypo(TrueIdentity.LastName);
		break;

	case 5:
		//Different Last Name
		if (LastNames.Num() > 0)
		{
			ForgedPassport.LastName = LastNames[FMath::RandRange(0, LastNames.Num() - 1)];
		}
		break;

	case 6:
		//Completely Fake ID Number
		ForgedPassport.IDNumber = FString::Printf(TEXT("%06d"), FMath::RandRange(100000, 999999));
		break;

	case 7:
		//Typo in Date of Birth
		ForgedPassport.DOB = GenerateTypo(TrueIdentity.DOB);
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

FString ADataManager::GetStopNameByIndex(int32 Index)
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
		int32 SwapIndex = FMath::RandRange(0, i);
		ShuffledStops.Swap(i, SwapIndex);
	}

	int32 StopsToPick = FMath::Min(NumberOfStops, ShuffledStops.Num());

	for (int i = 0; i < StopsToPick; i++)
	{
		ActiveRoute.Add(ShuffledStops[i]);
	}
}

TArray<FPassengerRecord> ADataManager::GeneratePassengerQueue(int32 TotalPassengers, int32 MinImposters, int32 MaxImposters)
{
	TArray<FPassengerRecord> DailyQueue;

	int32 TargetImposters = FMath::RandRange(MinImposters, MaxImposters);
	int32 CurrentImposters = 0;

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

		//In case we do multiple days?
		//if (CurrentImposters < TargetImposters)
		//{
		//	NewRecord.bIsImposter = true;

		//	if (CurrentDay == 1)
		//	{
		//		//DAY 1: Only passports can be forged, Tickets always valid
		//		NewRecord.PresentedPassport = GenerateImposterPassport(NewRecord.TrueIdentity);
		//		NewRecord.PresentedTicket = GenerateValidTicket();
		//	}
		//	else if (CurrentDay >= 2)
		//	{
		//		//DAY 2: 50/50 chance to forge either Passport or Ticket
		//		if (FMath::RandBool())
		//		{
		//			//Fake Passport, Valid Ticket
		//			NewRecord.PresentedPassport = GenerateImposterPassport(NewRecord.TrueIdentity);
		//			NewRecord.PresentedTicket = GenerateValidTicket();
		//		}
		//		else
		//		{
		//			//Valid Passport, Fake Ticket
		//			NewRecord.PresentedPassport = NewRecord.TrueIdentity;
		//			NewRecord.PresentedTicket = GenerateFakeTicket(GenerateValidTicket());
		//		}
		//	}

		//	CurrentImposters++;
		//}
		//else
		//{
		//	//Everything is valid
		//	NewRecord.bIsImposter = false;
		//	NewRecord.PresentedPassport = NewRecord.TrueIdentity;
		//	NewRecord.PresentedTicket = GenerateValidTicket();
		//}

		DailyQueue.Add(NewRecord);
	}

	for (int i = DailyQueue.Num() - 1; i > 0; i--)
	{
		int SwapIndex = FMath::RandRange(0, i);
		DailyQueue.Swap(i, SwapIndex);
	}

	return DailyQueue;
}

FTicketData ADataManager::GenerateValidTicket()
{
	FTicketData NewTicket;

	NewTicket.TicketDate = CurrentGameDate;

	if (ActiveRoute.Num() >= 2)
	{
		int IndexA = FMath::RandRange(0, ActiveRoute.Num() - 1);
		int IndexB = FMath::RandRange(0, ActiveRoute.Num() - 1);

		while (IndexA == IndexB)
		{
			IndexB = FMath::RandRange(0, ActiveRoute.Num() - 1);
		}

		int32 BoardingIndex = FMath::Min(IndexA, IndexB);
		int32 FinalIndex = FMath::Max(IndexA, IndexB);

		NewTicket.BoardingStop = ActiveRoute[BoardingIndex];
		NewTicket.FinalStop = ActiveRoute[FinalIndex];
	}
	else
	{
		NewTicket.BoardingStop = TEXT("Error_Start");
		NewTicket.FinalStop = TEXT("Error_End");
	}

	return NewTicket;
}

FTicketData ADataManager::GenerateFakeTicket(FTicketData ValidTicket)
{
	FTicketData FakeTicket = ValidTicket;

	int DiscrepancyType = FMath::RandRange(1, 3);

	switch (DiscrepancyType)
	{
	case 1:
		//Wrong Date
		FakeTicket.TicketDate = GenerateTypo(ValidTicket.TicketDate);
		break;

	case 2:
	case 3:
	{
		//Wrong Stop
		FString InvalidStop = TEXT("Unknown Station");

		for (const FString& Stop : BusStopNames)
		{
			if (!ActiveRoute.Contains(Stop))
			{
				InvalidStop = Stop;
				break;
			}
		}

		if (DiscrepancyType == 2)
		{
			FakeTicket.BoardingStop = InvalidStop;
		}
		else
		{
			FakeTicket.FinalStop = InvalidStop;
		}
		break;
	}
	}

	return FakeTicket;
}
