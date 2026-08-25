// Fill out your copyright notice in the Description page of Project Settings.

#include "BusStop.h"
#include "Components/BoxComponent.h"

ABusStop::ABusStop()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(200.0f, 200.0f, 50.0f));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector ABusStop::GetRandomSpawnPoint() const
{
	const FVector Extent = SpawnArea->GetScaledBoxExtent();

	const FVector RandomLocalOffset(
		FMath::FRandRange(-Extent.X, Extent.X),
		FMath::FRandRange(-Extent.Y, Extent.Y),
		FMath::FRandRange(-Extent.Z, Extent.Z));

	return SpawnArea->GetComponentTransform().TransformPosition(RandomLocalOffset);
}
