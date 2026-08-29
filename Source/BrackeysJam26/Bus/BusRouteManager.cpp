// Fill out your copyright notice in the Description page of Project Settings.

#include "BusRouteManager.h"
#include "../DataManager.h"
#include "../Components/BusQueueComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BrackeysJam26/Character/PlayerCharacter.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "../HUD/MenuFlowSubsystem.h"

ABusRouteManager::ABusRouteManager()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentStopIndex = 0;
	CurrentState = ETransitionState::Idle;
	TransitionDuration = 3.0f;

	StartTimer();
}

void ABusRouteManager::StartDeparture()
{
	if (!BusReference || CurrentState != ETransitionState::Idle) return;

	CurrentState = ETransitionState::Departing;
	TransitionAlpha = 0.0f;

	StartLocation = BusReference->GetActorLocation();

	TargetLocation = StartLocation + (BusReference->GetActorRightVector() * 750.0f);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		Player->SetLoadingState(true);
	}

	OnDepart.Broadcast();
}

FString ABusRouteManager::GetFormattedTimeRemaining()
{
	bool bIsNegative = TimeRemaining < 0.0f;

	float AbsoluteTime = FMath::Abs(TimeRemaining);

	int32 Minutes = FMath::FloorToInt(AbsoluteTime / 60.0f);
	int32 Seconds = FMath::FloorToInt(FMath::Fmod(AbsoluteTime, 60.0f));

	FString SignString = bIsNegative ? TEXT("-") : TEXT("");
	return FString::Printf(TEXT("%s%02d:%02d"), *SignString, Minutes, Seconds);
}

void ABusRouteManager::EvaluateEndGame()
{
	int32 ImpostersOnBus = 0;

	TArray<AActor*> AllNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPCCharacter::StaticClass(), AllNPCs);

	for (AActor* Actor : AllNPCs)
	{
		if (ANPCCharacter* NPC = Cast<ANPCCharacter>(Actor))
		{
			if (NPC->IsSitting() && NPC->PassengerRecord.bIsImposter)
			{
				ImpostersOnBus++;
			}
		}
	}

	UMenuFlowSubsystem* MenuFlow = GetGameInstance()->GetSubsystem<UMenuFlowSubsystem>();
	if (!MenuFlow) return;

	if (ImpostersOnBus > 0 || RealNPCsDenied > 1)
	{
		MenuFlow->SetRootState(LoseMenuState);
	}
	else
	{
		MenuFlow->SetRootState(WinMenuState);
	}
}

void ABusRouteManager::PerformTeleport()
{
	

	if (DataManagerReference)
	{
		FString NextName = DataManagerReference->GetStopNameByIndex(CurrentStopIndex);

		BP_UpdateStopScreen(NextName);
	}

	if (!TeleportPoints.IsValidIndex(CurrentStopIndex) || !FinalStopPoints.IsValidIndex(CurrentStopIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("PerformTeleport: invalid index %d (TeleportPoints=%d, FinalStopPoints=%d)"),
			CurrentStopIndex, TeleportPoints.Num(), FinalStopPoints.Num());
	}

	if (TeleportPoints.IsValidIndex(CurrentStopIndex) && FinalStopPoints.IsValidIndex(CurrentStopIndex))
	{
		FTransform NewTransform = TeleportPoints[CurrentStopIndex]->GetActorTransform();
		BusReference->SetActorTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);


		StartLocation = BusReference->GetActorLocation();
		TargetLocation = FinalStopPoints[CurrentStopIndex]->GetActorLocation();

		TransitionAlpha = 0.0f;
		CurrentState = ETransitionState::Arriving;

		if (auto* Queue = BusReference->FindComponentByClass<UBusQueueComponent>())
		{
			Queue->SpawnPassengersForStop(BusStops.IsValidIndex(CurrentStopIndex) ? BusStops[CurrentStopIndex] : nullptr);
		}
	}

	CurrentStopIndex++;
	AddStopBonusTime();
}

void ABusRouteManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTimerActive)
	{
		TimeRemaining = FMath::Max(TimeRemaining - DeltaTime, MaxNegativeTime);

		if (TimeRemaining <= MaxNegativeTime)
		{
			UMenuFlowSubsystem* MenuFlowSubsystem = GetGameInstance()->GetSubsystem<UMenuFlowSubsystem>();

			if (MenuFlowSubsystem)
			{
				MenuFlowSubsystem->SetRootState(MenuState);
			}
		}
	}

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

			if (CurrentState != ETransitionState::Idle)
			{
				CurrentState = ETransitionState::Loading;
				CurrentLoadTime = 0.0f;

				int32 LegIndex = FMath::Clamp(CurrentStopIndex - 1, 0, LegLoadingImages.Num() - 1);
				UTexture2D* CurrentImage = LegLoadingImages.IsValidIndex(LegIndex) ? LegLoadingImages[LegIndex] : nullptr;

				BP_ShowLoadingScreenForLeg(CurrentImage, LegIndex);
			}
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

			if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				Player->SetLoadingState(false);
			}

			OnArrive.Broadcast();

			if (CurrentStopIndex >= TeleportPoints.Num())
			{
				EvaluateEndGame();
			}
		}
	}
}

void ABusRouteManager::AddStopBonusTime()
{
	float BonusTime = 120.0f;

	TimeRemaining += BonusTime;
}