// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataManager.generated.h"

USTRUCT(BlueprintType)
struct FPassengerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FirstName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LastName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IDNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DOB;
};

USTRUCT(BlueprintType)
struct FTicketData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BoardingStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FinalStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TicketDate;
};

USTRUCT(BlueprintType)
struct FPassengerRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPassengerData TrueIdentity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPassengerData PresentedPassport;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTicketData TrueTicket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTicketData PresentedTicket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsImposter = false;
};

UCLASS()
class BRACKEYSJAM26_API ADataManager : public AActor
{
	GENERATED_BODY()

public:
	ADataManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Lists")
	TArray<FString> FirstNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Lists")
	TArray<FString> LastNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Lists")
	TArray<FString> IDNumbers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Lists")
	TArray<FString> BusStopNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Lists")
	TArray<FString> ActiveRoute;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Lists")
	int NumberOfStops = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Date")
	FString CurrentGameDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 CurrentDay = 1;


	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FPassengerData GenerateRandomPassenger();

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FPassengerData GenerateImposterPassport(FPassengerData TrueIdentity);

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FString GetStopNameByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	void GenerateActiveRoute();

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	TArray<FPassengerRecord> GeneratePassengerQueue(int32 TotalPassengers, int32 MinImposters, int32 MaxImposters);

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FTicketData GenerateValidTicket();

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FTicketData GenerateFakeTicket(FTicketData ValidTicket);

private:
	FString GenerateTypo(FString OriginalText);

	void LoadTextFileIntoArray(FString FileName, TArray<FString>& OutArray);
};
