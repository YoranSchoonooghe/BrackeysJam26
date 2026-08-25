#include "BusSeat.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "Components/WidgetComponent.h"
#include "BusSeatButtonWidget.h"

ABusSeat::ABusSeat()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	EjectButtonWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EjectButtonWidget"));
	EjectButtonWidget->SetupAttachment(RootComponent);
	EjectButtonWidget->SetWidgetSpace(EWidgetSpace::World);
	EjectButtonWidget->SetDrawSize(FVector2D(100.0f, 50.0f));
	EjectButtonWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	EjectButtonWidget->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	EjectButtonWidget->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	EjectButtonWidget->SetVisibility(false);
}

void ABusSeat::BeginPlay()
{
	Super::BeginPlay();

	if (UBusSeatButtonWidget* Widget = Cast<UBusSeatButtonWidget>(EjectButtonWidget->GetWidget()))
	{
		Widget->OnPressed.AddDynamic(this, &ABusSeat::Eject);
	}
}

//void ABusSeat::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ABusSeat::Occupy(ANPCCharacter* NPCCharacter)
{
	if (IsOccupied()) return;
	if (!NPCCharacter) return;

	Occupant = NPCCharacter;
	Occupant->SetCurrentSeat(this);
}

void ABusSeat::Leave()
{
	if (!IsOccupied()) return;

	Occupant = nullptr;
}

void ABusSeat::Eject()
{
	if (!IsOccupied()) return;

	if (Occupant->Eject(EjectionForce))
	{
		EjectButtonWidget->SetVisibility(false);
	}
}

bool ABusSeat::IsOccupied() const
{
	return (Occupant != nullptr);
}

void ABusSeat::SetButtonVisible(bool bVisible)
{
	EjectButtonWidget->SetVisibility(bVisible && IsOccupied());
}