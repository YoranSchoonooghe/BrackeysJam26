// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"
#include "Math/UnrealMathUtility.h"

namespace
{
	const TArray<FString>& GetFallbackFirstNames()
	{
		static const TArray<FString> Names = {
			TEXT("Axel"), TEXT("Raven"), TEXT("Salem"), TEXT("Blair"), TEXT("Eden"), TEXT("Sage"), TEXT("Nova"),
			TEXT("River"), TEXT("Storm"), TEXT("Skye"), TEXT("Quinn"), TEXT("Darcy"), TEXT("Reese"), TEXT("Juno"),
			TEXT("Cleo"), TEXT("Remy"), TEXT("Ezra"), TEXT("Zion"), TEXT("Vale"), TEXT("Gray"), TEXT("Mara"),
			TEXT("Iris"), TEXT("Vera"), TEXT("Elias"), TEXT("Silas"), TEXT("Nolan"), TEXT("Daria"), TEXT("Elian"),
			TEXT("Ariel"), TEXT("Roman"), TEXT("Nadia"), TEXT("Soren"), TEXT("Livia"), TEXT("Orion"), TEXT("Gary"),
			TEXT("Boris"), TEXT("Michael"), TEXT("Arlen"), TEXT("Ansel"), TEXT("Elio"), TEXT("Ivo"), TEXT("Oren"),
			TEXT("Orin"), TEXT("Remy"), TEXT("Soren"), TEXT("Vex"), TEXT("Rune"), TEXT("Echo"), TEXT("Zero"),
			TEXT("Seven"), TEXT("Salem"), TEXT("Nyx"), TEXT("Lux"), TEXT("Vale"), TEXT("Fable"), TEXT("Onyx"),
			TEXT("Astra"), TEXT("Nova"), TEXT("Sol"), TEXT("Mars"), TEXT("Eden"), TEXT("Halo"), TEXT("Peter"),
			TEXT("Carl"), TEXT("Mark"), TEXT("Luke"), TEXT("Stefan"), TEXT("Tomas"), TEXT("Arthur"), TEXT("Amanda"),
			TEXT("Helen"), TEXT("Leah"), TEXT("Leon")
		};
		return Names;
	}

	const TArray<FString>& GetFallbackLastNames()
	{
		static const TArray<FString> Names = {
			TEXT("Miller"), TEXT("Carter"), TEXT("Walker"), TEXT("Foster"), TEXT("Parker"), TEXT("Turner"),
			TEXT("Cooper"), TEXT("Baker"), TEXT("Morgan"), TEXT("Hunter"), TEXT("Wilson"), TEXT("Palmer"),
			TEXT("Harris"), TEXT("Mason"), TEXT("Brooks"), TEXT("Bennett"), TEXT("Collins"), TEXT("Murphy"),
			TEXT("Dawson"), TEXT("Mercer"), TEXT("Novak"), TEXT("Kowal"), TEXT("Urban"), TEXT("Marek"),
			TEXT("Varga"), TEXT("Horvat"), TEXT("Novak"), TEXT("Kolar"), TEXT("Jovan"), TEXT("Petrov"),
			TEXT("Volkov"), TEXT("Moroz"), TEXT("Kozak"), TEXT("Orlov"), TEXT("Zoric"), TEXT("Markov"),
			TEXT("Pavlov"), TEXT("Sokol"), TEXT("Drago"), TEXT("Vitek"), TEXT("Jackson"), TEXT("Fisher"),
			TEXT("Adams"), TEXT("Ford"), TEXT("Ellis"), TEXT("Gray"), TEXT("Scott"), TEXT("Ross"), TEXT("Smith"),
			TEXT("Young"), TEXT("Reed"), TEXT("Green"), TEXT("Ramsey"), TEXT("Preston"), TEXT("Körgsen")
		};
		return Names;
	}

	const TArray<FString>& GetFallbackBusStops()
	{
		static const TArray<FString> Stops = {
			TEXT("Bad Idea"), TEXT("Don't Look"), TEXT("Welcome Back"), TEXT("You Again"), TEXT("No Exit"),
			TEXT("The Void"), TEXT("Gary St"), TEXT("Lost St"), TEXT("Loop St"), TEXT("Again St"),
			TEXT("Same Place"), TEXT("Old Stop"), TEXT("Last Bus"), TEXT("Bus Stop?"), TEXT("Why 13?"),
			TEXT("Not Again"), TEXT("Same Again"), TEXT("You Know"), TEXT("Remember?"), TEXT("???"),
			TEXT("Unknown"), TEXT("Nowhere"), TEXT("Somewhere"), TEXT("Elsewhere"), TEXT("The Void"),
			TEXT("No Name"), TEXT("Secret St"), TEXT("Hidden St"), TEXT("It Saw You"), TEXT("You Again"),
			TEXT("Wrong Stop"), TEXT("Last Stop"), TEXT("No Exit"), TEXT("Dead End"), TEXT("Bad Place"),
			TEXT("Dark Road"), TEXT("Empty Road"), TEXT("Lost Town"), TEXT("Ghost St"), TEXT("Dead St"),
			TEXT("Quiet St"), TEXT("Silent St"), TEXT("No One Here"), TEXT("Stay Inside"), TEXT("Don't Look"),
			TEXT("Don't Stop"), TEXT("Turn Back"), TEXT("No Way"), TEXT("Go Away"), TEXT("Not Here"),
			TEXT("Wrong Way"), TEXT("Sunshine Park"), TEXT("Hedge stop"), TEXT("The empty nest"),
			TEXT("Empty Lane"), TEXT("Livingstone"), TEXT("Deadstone"), TEXT("Bus Stopn't"), TEXT("Area 51½"),
			TEXT("Your Stop"), TEXT("Bro What's That"), TEXT("Swear It's Safe"), TEXT("Don't Ask"),
			TEXT("Don't Look"), TEXT("Don't Get Off"), TEXT("The Mistake"), TEXT("Oops"), TEXT("Oops Again"),
			TEXT("Oh No"), TEXT("Oh No 2")
		};
		return Stops;
	}
}

ADataManager::ADataManager()
{
	PrimaryActorTick.bCanEverTick = false;

	FDateTime Today = FDateTime::Now();
	CurrentGameDate = FString::Printf(TEXT("%04d-%02d-%02d"), Today.GetYear(), Today.GetMonth(), Today.GetDay());

	FirstNames = GetFallbackFirstNames();
	LastNames = GetFallbackLastNames();
	BusStopNames = GetFallbackBusStops();

	GenerateActiveRoute();
}

void ADataManager::BeginPlay()
{
	Super::BeginPlay();
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
	if (OriginalText.Len() <= 3) return OriginalText;

	TArray<int32> ValidIndices;
	for (int32 i = 0; i < OriginalText.Len() - 1; i++)
	{
		if (OriginalText[i] != OriginalText[i + 1])
		{
			ValidIndices.Add(i);
		}
	}

	if (ValidIndices.IsEmpty())
	{
		return OriginalText;
	}

	int32 SwapIndex = ValidIndices[FMath::RandRange(0, ValidIndices.Num() - 1)];

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
		NewRecord.TrueTicket = GenerateValidTicket();

		if (CurrentImposters < TargetImposters)
		{
			NewRecord.bIsImposter = true;
			if (FMath::RandBool())
			{
				//Fake Passport, Valid Ticket
				NewRecord.PresentedPassport = GenerateImposterPassport(NewRecord.TrueIdentity);
				NewRecord.PresentedTicket = NewRecord.TrueTicket;
			}
			else
			{
				//Valid Passport, Fake Ticket
				NewRecord.PresentedPassport = NewRecord.TrueIdentity;
				NewRecord.PresentedTicket = GenerateFakeTicket(NewRecord.TrueTicket);
			}
			CurrentImposters++;
		}
		else
		{
			NewRecord.bIsImposter = false;
			NewRecord.PresentedPassport = NewRecord.TrueIdentity;
			NewRecord.PresentedTicket = NewRecord.TrueTicket;
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
