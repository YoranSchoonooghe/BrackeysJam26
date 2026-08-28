// Fill out your copyright notice in the Description page of Project Settings.

#include "BusStop.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"

ABusStop::ABusStop()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(200.0f, 200.0f, 50.0f));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	VisualBusStop = CreateDefaultSubobject<UChildActorComponent>(TEXT("VisualBusStop"));
	VisualBusStop->SetupAttachment(RootComponent);
}

void ABusStop::BeginPlay()
{
	Super::BeginPlay();

	if (!BusStopBlueprints.IsEmpty())
	{
		int32 RandomIndex = FMath::RandRange(0, BusStopBlueprints.Num() - 1);
		VisualBusStop->SetChildActorClass(BusStopBlueprints[RandomIndex]);
	}
}

FVector ABusStop::GetRandomSpawnPoint() const
{
	for (int32 Attempt = 0; Attempt < MaxSpawnAttempts; ++Attempt)
	{
		const FVector Candidate = GetRandomPointInArea();

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		const bool bBlocked = GetWorld()->OverlapAnyTestByChannel(
			Candidate,
			FQuat::Identity,
			ECC_Pawn,
			FCollisionShape::MakeSphere(SpawnCheckRadius),
			Params);

		if (!bBlocked)
		{
			return Candidate;
		}
	}

	return SpawnArea->GetComponentLocation();
}

FVector ABusStop::GetRandomPointInArea() const
{
	const FVector Extent = SpawnArea->GetScaledBoxExtent();
	const FVector RandomLocalOffset(
		FMath::FRandRange(-Extent.X, Extent.X),
		FMath::FRandRange(-Extent.Y, Extent.Y),
		FMath::FRandRange(-Extent.Z, Extent.Z));
	return SpawnArea->GetComponentTransform().TransformPosition(RandomLocalOffset);
}