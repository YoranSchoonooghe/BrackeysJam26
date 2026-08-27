#include "BusSeat.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "Components/WidgetComponent.h"
#include "BusSeatButtonWidget.h"
#include "BrackeysJam26/Character/DefaultPlayerController.h"
#include <Kismet/GameplayStatics.h>

ABusSeat::ABusSeat()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	EjectButtonWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EjectButtonWidget"));
	EjectButtonWidget->SetupAttachment(RootComponent);
	EjectButtonWidget->SetWidgetSpace(EWidgetSpace::World);
	EjectButtonWidget->SetDrawSize(FVector2D(100.0f, 50.0f));
	EjectButtonWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	EjectButtonWidget->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	EjectButtonWidget->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	EjectButtonWidget->SetVisibility(false);
	EjectButtonWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InfoButtonWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoButtonWidget"));
	InfoButtonWidget->SetupAttachment(RootComponent);
	InfoButtonWidget->SetWidgetSpace(EWidgetSpace::World);
	InfoButtonWidget->SetDrawSize(FVector2D(100.0f, 50.0f));
	InfoButtonWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	InfoButtonWidget->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	InfoButtonWidget->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	InfoButtonWidget->SetVisibility(false);
	InfoButtonWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABusSeat::BeginPlay()
{
	Super::BeginPlay();

	if (UBusSeatButtonWidget* Widget = Cast<UBusSeatButtonWidget>(EjectButtonWidget->GetWidget()))
	{
		Widget->OnPressed.AddDynamic(this, &ABusSeat::Eject);
	}

	if (UBusSeatButtonWidget* Widget = Cast<UBusSeatButtonWidget>(InfoButtonWidget->GetWidget()))
	{
		Widget->OnPressed.AddDynamic(this, &ABusSeat::ShowCharacterInfo);
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
		SetButtonVisible(false);

		Occupant = nullptr;

		OnPassengerEjected.Broadcast();
	}
}

void ABusSeat::ShowCharacterInfo()
{
	if (!IsOccupied()) return;

	if (auto* PC = Cast<ADefaultPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->ShowCharacterInfoWidget(Occupant->PassengerRecord);
	}
}

bool ABusSeat::IsOccupied() const
{
	return (Occupant != nullptr);
}

void ABusSeat::SetButtonVisible(bool bVisible)
{
	const bool bShow = bVisible && IsOccupied();
	const ECollisionEnabled::Type CollisionType = bShow ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;

	EjectButtonWidget->SetVisibility(bShow);
	EjectButtonWidget->SetCollisionEnabled(CollisionType);

	InfoButtonWidget->SetVisibility(bShow);
	InfoButtonWidget->SetCollisionEnabled(CollisionType);
}

bool ABusSeat::IsButtonVisible() const
{
	return EjectButtonWidget->IsVisible();
}