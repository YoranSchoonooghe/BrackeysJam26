// Fill out your copyright notice in the Description page of Project Settings.

#include "MonitorActor.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "BrackeysJam26/Character/PlayerCharacter.h"

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

void AMonitorActor::BlendToTargetCamera()
{
	if (!TargetCamera) return;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetViewTargetWithBlend(TargetCamera, CameraBlendTime);
	}
}

void AMonitorActor::BlendToPlayerCamera()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetViewTargetWithBlend(PC->GetPawn(), CameraBlendTime);
	}
}

void AMonitorActor::Interact_Implementation()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	auto* PlayerCharacter = Cast<APlayerCharacter>(PC->GetPawn());

	if (PC->GetViewTarget() == this)
	{
		PC->SetViewTargetWithBlend(PC->GetPawn(), FocusBlendTime);
		if (PlayerCharacter)
		{
			PlayerCharacter->SetInputLocked(false);
		}
		ScreenWidgetComponent->SetVisibility(false);
	}
	else
	{
		PC->SetViewTargetWithBlend(this, FocusBlendTime);
		if (PlayerCharacter)
		{
			PlayerCharacter->SetInputLocked(true);
		}
		ScreenWidgetComponent->SetVisibility(true);
	}
}
