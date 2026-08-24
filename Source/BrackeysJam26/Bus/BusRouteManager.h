// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BusRouteManager.generated.h"


class ADataManager;

UENUM(BlueprintType)
enum class ETransitionState : uint8
{
    Idle,
    Departing,
	Loading,
    Arriving
};

UCLASS()
class BRACKEYSJAM26_API ABusRouteManager : public AActor
{
    GENERATED_BODY()

public:
	ABusRouteManager();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Route")
	TArray<AActor*> TeleportPoints;

	UPROPERTY(EditAnywhere, Category = "Route")
	TArray<AActor*> FinalStopPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	AActor* BusReference;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> FadeWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Route")
	int32 CurrentStopIndex;

	UFUNCTION(BlueprintCallable, Category = "Transition")
	void StartDeparture();

	UFUNCTION(BlueprintImplementableEvent, Category = "Transition")
	void BP_SetUIFade(float FadeAlpha);

	UPROPERTY(EditAnywhere, Category = "Route")
	float LoadingScreenDuration = 3.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Transition")
	void BP_ToggleLoadingImage(bool bShowImage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	ADataManager* DataManagerReference;

	UFUNCTION(BlueprintImplementableEvent, Category = "Route")
	void BP_UpdateStopScreen(const FString& NextStopName);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
	float TimeRemaining = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
	float MaxNegativeTime = -120.f;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer() { bIsTimerActive = true; }

	UFUNCTION(BlueprintCallable, Category = "Timer")
	FString GetFormattedTimeRemaining();

private:
	void PerformTeleport();

	void AddStopBonusTime();

	ETransitionState CurrentState;

	FVector StartLocation;
	FVector TargetLocation;

	float TransitionAlpha;
	float TransitionDuration;

	float CurrentLoadTime;

	bool bIsTimerActive = false;
};
