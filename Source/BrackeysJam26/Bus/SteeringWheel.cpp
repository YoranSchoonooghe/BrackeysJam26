// Fill out your copyright notice in the Description page of Project Settings.


#include "SteeringWheel.h"
#include "Bus.h"

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
		if (ABus* Bus = Cast<ABus>(BusToAttachTo))
		{
			if (!Bus->AreDoorsOpen())
			{
				RouteManager->StartDeparture();
			}
		}
	}
}

FText ASteeringWheel::GetPromptText_Implementation()
{
	if (RouteManager)
	{
		if (ABus* Bus = Cast<ABus>(BusToAttachTo))
		{
			if (!Bus->AreDoorsOpen())
			{
				return FText::FromString("Drive To Next Stop");
			}
		}
	}
	return FText::FromString("Wait For Passengers");
}