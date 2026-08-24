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
	FString Species;
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
	TArray<FString> SpeciesList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Lists")
	TArray<FString> BusStopNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Lists")
	TArray<FString> ActiveRoute;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Lists")
	int NumberOfStops = 5;


	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FPassengerData GenerateRandomPassenger();

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FPassengerData GenerateImposterPassport(FPassengerData TrueIdentity);

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	FString GetStopNameByIndex(int Index);

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	void GenerateActiveRoute();

	UFUNCTION(BlueprintCallable, Category = "Data|Generation")
	TArray<FPassengerRecord> GeneratePassengerQueue(int TotalPassengers, int MinImposters, int MaxImposters);

private:
	FString GenerateTypo(FString OriginalText);

	void LoadTextFileIntoArray(FString FileName, TArray<FString>& OutArray);
};
