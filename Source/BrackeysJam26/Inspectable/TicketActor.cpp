#include "TicketActor.h"
#include "Components/TextRenderComponent.h"

ATicketActor::ATicketActor()
{
	PrimaryActorTick.bCanEverTick = false;

	auto* pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = pRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	BoardingStopText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BoardingStop"));
	BoardingStopText->SetupAttachment(RootComponent);

	FinalStopText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("FinalStop"));
	FinalStopText->SetupAttachment(RootComponent);

	DateText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TicketDate"));
	DateText->SetupAttachment(RootComponent);
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

void ATicketActor::UpdateTicket(const FTicketData& TicketData)
{
	BoardingStopText->SetText(FText::FromString(TicketData.BoardingStop));
	FinalStopText->SetText(FText::FromString(TicketData.FinalStop));
	DateText->SetText(FText::FromString(TicketData.TicketDate));
}