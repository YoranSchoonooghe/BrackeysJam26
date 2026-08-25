// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BusSeatButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSeatButtonPressed);

UCLASS()
class BRACKEYSJAM26_API UBusSeatButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Seat")
	FOnSeatButtonPressed OnPressed;

	UFUNCTION(BlueprintCallable, Category = "Seat")
	void PressButton();
};
