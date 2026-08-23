#include "Bus.h"

ABus::ABus()
{
	PrimaryActorTick.bCanEverTick = false;

	auto* pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = pRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
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

