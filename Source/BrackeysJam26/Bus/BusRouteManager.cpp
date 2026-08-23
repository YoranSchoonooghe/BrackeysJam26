// Fill out your copyright notice in the Description page of Project Settings.


#include "BusRouteManager.h"

ABusRouteManager::ABusRouteManager()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentStopIndex = 0;
	CurrentState = ETransitionState::Idle;
	TransitionDuration = 3.0f;
}

void ABusRouteManager::StartDeparture()
{
	if (!BusReference || CurrentState != ETransitionState::Idle) return;

	CurrentState = ETransitionState::Departing;
	TransitionAlpha = 0.0f;

	StartLocation = BusReference->GetActorLocation();

	TargetLocation = StartLocation + (BusReference->GetActorRightVector() * 750.0f);
}

void ABusRouteManager::PerformTeleport()
{
	CurrentStopIndex++;

	if (TeleportPoints.IsValidIndex(CurrentStopIndex) && FinalStopPoints.IsValidIndex(CurrentStopIndex))
	{
		FTransform NewTransform = TeleportPoints[CurrentStopIndex]->GetActorTransform();
		BusReference->SetActorTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);

		StartLocation = BusReference->GetActorLocation();
		TargetLocation = FinalStopPoints[CurrentStopIndex]->GetActorLocation();

		TransitionAlpha = 0.0f;
		CurrentState = ETransitionState::Arriving;
	}
}

void ABusRouteManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState == ETransitionState::Idle || !BusReference) return;

	if (CurrentState == ETransitionState::Departing)
	{
		TransitionAlpha += (DeltaTime / TransitionDuration);
		float ClampedAlpha = FMath::Clamp(TransitionAlpha, 0.0f, 1.0f);

		FVector NewLoc = FMath::InterpEaseInOut(StartLocation, TargetLocation, ClampedAlpha, 2.0f);
		BusReference->SetActorLocation(NewLoc);

		BP_SetUIFade(ClampedAlpha);

		if (ClampedAlpha >= 1.0f)
		{
			PerformTeleport();

			CurrentState = ETransitionState::Loading;
			CurrentLoadTime = 0.0f;
			BP_ToggleLoadingImage(true);
		}
	}
	else if (CurrentState == ETransitionState::Loading)
	{
		CurrentLoadTime += DeltaTime;

		if (CurrentLoadTime >= LoadingScreenDuration)
		{
			CurrentState = ETransitionState::Arriving;
			BP_ToggleLoadingImage(false);
		}
	}
	else if (CurrentState == ETransitionState::Arriving)
	{
		TransitionAlpha += (DeltaTime / TransitionDuration);
		float ClampedAlpha = FMath::Clamp(TransitionAlpha, 0.0f, 1.0f);

		FVector NewLoc = FMath::InterpEaseInOut(StartLocation, TargetLocation, ClampedAlpha, 2.0f);
		BusReference->SetActorLocation(NewLoc);

		BP_SetUIFade(1.0f - ClampedAlpha);

		if (ClampedAlpha >= 1.0f)
		{
			CurrentState = ETransitionState::Idle;
		}
	}
}