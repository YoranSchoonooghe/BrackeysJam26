// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BusStop.generated.h"

class UBoxComponent;
class UChildActorComponent;

UCLASS()
class BRACKEYSJAM26_API ABusStop : public AActor
{
	GENERATED_BODY()

public:
	ABusStop();

	UFUNCTION(BlueprintCallable, Category = "Bus Stop")
	FVector GetRandomSpawnPoint() const;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> SpawnArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UChildActorComponent> VisualBusStop;

	UPROPERTY(EditAnywhere, Category = "Visuals")
	TArray<TSubclassOf<AActor>> BusStopBlueprints;
};
