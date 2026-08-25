// Fill out your copyright notice in the Description page of Project Settings.

#include "MonitorActor.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

AMonitorActor::AMonitorActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ScreenWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScreenWidget"));
	ScreenWidgetComponent->SetupAttachment(RootComponent);
	ScreenWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	ScreenWidgetComponent->SetDrawSize(FVector2D(500.0f, 300.0f));
	ScreenWidgetComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
