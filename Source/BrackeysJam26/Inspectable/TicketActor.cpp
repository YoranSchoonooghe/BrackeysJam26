#include "TicketActor.h"

ATicketActor::ATicketActor()
{
	PrimaryActorTick.bCanEverTick = true;

	auto* pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = pRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ATicketActor::BeginPlay()
{
	Super::BeginPlay();
	
}

//void ATicketActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

