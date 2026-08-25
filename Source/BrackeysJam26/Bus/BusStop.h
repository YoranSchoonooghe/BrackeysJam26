// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BusStop.generated.h"

class UBoxComponent;

UCLASS()
class BRACKEYSJAM26_API ABusStop : public AActor
{
	GENERATED_BODY()

public:
	ABusStop();

	UFUNCTION(BlueprintCallable, Category = "Bus Stop")
	FVector GetRandomSpawnPoint() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> SpawnArea;
};
