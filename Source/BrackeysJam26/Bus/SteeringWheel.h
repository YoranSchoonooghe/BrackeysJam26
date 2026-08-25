// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../InteractableInterface.h"
#include "BusRouteManager.h"
#include "SteeringWheel.generated.h"

UCLASS()
class BRACKEYSJAM26_API ASteeringWheel : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ASteeringWheel();

	void BeginPlay();

	UPROPERTY(EditAnywhere, Category = "Routing")
	ABusRouteManager* RouteManager;

	UPROPERTY(EditAnywhere, Category = "Attachment")
	AActor* BusToAttachTo;

	virtual void Interact_Implementation() override;

	virtual FText GetPromptText_Implementation() override;
};
