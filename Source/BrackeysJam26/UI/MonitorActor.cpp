// Fill out your copyright notice in the Description page of Project Settings.

#include "MonitorActor.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "BrackeysJam26/Character/PlayerCharacter.h"
#include "BrackeysJam26/Bus/Bus.h"
#include "BrackeysJam26/Bus/BusSeat.h"

AMonitorActor::AMonitorActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ScreenWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScreenWidget"));
	ScreenWidgetComponent->SetupAttachment(RootComponent);
	ScreenWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	ScreenWidgetComponent->SetDrawSize(FVector2D(500.0f, 300.0f));
	ScreenWidgetComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ScreenWidgetComponent->SetVisibility(false);

	FocusCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FocusCamera"));
	FocusCameraComponent->SetupAttachment(RootComponent);
	FocusCameraComponent->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));
	FocusCameraComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
}

void AMonitorActor::ShowScreen(EMonitorScreen Screen)
{
	if (UMonitorWidget* Widget = Cast<UMonitorWidget>(ScreenWidgetComponent->GetWidget()))
	{
		Widget->ShowScreen(Screen);
	}
}

void AMonitorActor::UpdateCharacterInfo(const FPassengerRecord& Record)
{
	if (UMonitorWidget* Widget = Cast<UMonitorWidget>(ScreenWidgetComponent->GetWidget()))
	{
		Widget->UpdateCharacterInfo(Record);
	}
}

void AMonitorActor::BlendToTargetCamera()
{
	if (!TargetCamera) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	PC->SetViewTargetWithBlend(TargetCamera, CameraBlendTime);

	if (auto* PlayerCharacter = Cast<APlayerCharacter>(PC->GetPawn()))
	{
		PlayerCharacter->SetInputLocked(true);
	}
}

void AMonitorActor::BlendToPlayerCamera()
{
	Close();
}

void AMonitorActor::SetSeatButtonsVisible(bool bVisible)
{
	auto* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
	if (!Bus) return;

	for (ABusSeat* Seat : Bus->GetSeats())
	{
		if (Seat)
		{
			Seat->SetButtonVisible(bVisible);
		}
	}
}

void AMonitorActor::Interact_Implementation()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	if (PC->GetViewTarget() == this)
	{
		Close();
		return;
	}

	PC->SetViewTargetWithBlend(this, FocusBlendTime);

	if (auto* PlayerCharacter = Cast<APlayerCharacter>(PC->GetPawn()))
	{
		PlayerCharacter->SetInputLocked(true);
	}

	ScreenWidgetComponent->SetVisibility(true);

	OnMonitorOpened.Broadcast();
}

void AMonitorActor::Close()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	if (PC->GetViewTarget() == this)
	{
		PC->SetViewTargetWithBlend(PC->GetPawn(), FocusBlendTime);
		ScreenWidgetComponent->SetVisibility(false);

		if (auto* PlayerCharacter = Cast<APlayerCharacter>(PC->GetPawn()))
		{
			PlayerCharacter->SetInputLocked(false);
		}
	}
	else
	{
		PC->SetViewTargetWithBlend(this, FocusBlendTime);
		SetSeatButtonsVisible(false);
		ScreenWidgetComponent->SetVisibility(true);

		auto* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
		if (Bus)
			Bus->SetRoofVisibility(true);
	}
}
