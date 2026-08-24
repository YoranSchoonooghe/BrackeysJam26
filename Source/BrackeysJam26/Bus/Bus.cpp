#include "Bus.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "BusSeat.h"

ABus::ABus()
{
	PrimaryActorTick.bCanEverTick = false;

	auto* pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = pRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	CheckLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CheckLocation"));
	CheckLocation->SetupAttachment(RootComponent);
	ExitLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ExitLocation"));
	ExitLocation->SetupAttachment(RootComponent);
}

void ABus::BeginPlay()
{
	Super::BeginPlay();
	
}

//void ABus::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

FVector ABus::GetCheckLocation() const
{
	return CheckLocation->GetComponentLocation();
}

FVector ABus::GetExitLocation() const
{
	return ExitLocation->GetComponentLocation();
}

ABusSeat* ABus::GetAvailableSeat() const
{
	if (Seats.IsEmpty()) return nullptr;

	return Seats[0];
}

