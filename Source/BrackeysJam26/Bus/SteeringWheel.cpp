// Fill out your copyright notice in the Description page of Project Settings.


#include "SteeringWheel.h"

ASteeringWheel::ASteeringWheel()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASteeringWheel::BeginPlay()
{
    Super::BeginPlay();

    if (BusToAttachTo)
    {
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
        AttachToActor(BusToAttachTo, AttachmentRules);
    }
}

void ASteeringWheel::Interact_Implementation()
{
	if (RouteManager)
	{
		RouteManager->StartDeparture();
	}
}

FText ASteeringWheel::GetPromptText_Implementation()
{
    return FText::FromString("Drive to next stop");
}