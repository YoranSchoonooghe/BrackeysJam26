#include "NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "BrackeysJam26/Bus/Bus.h"
#include "BrackeysJam26/Bus/BusSeat.h"


ANPCAIController::ANPCAIController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BTNPC);
	RefreshBusKeys();
}

void ANPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void ANPCAIController::RefreshBusKeys()
{
	auto* pBlackboardComponent = GetBlackboardComponent();
	if (!pBlackboardComponent) return;

	ABus* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
	if (!Bus) return;

	pBlackboardComponent->SetValueAsVector(TEXT("CheckLocation"), Bus->GetCheckLocation());
	pBlackboardComponent->SetValueAsVector(TEXT("ExitLocation"), Bus->GetExitLocation());
	pBlackboardComponent->SetValueAsVector(TEXT("DespawnLocation"), Bus->GetDespawnLocation());

	if (auto* Seat = Bus->GetAvailableSeat())
	{
		pBlackboardComponent->SetValueAsObject(TEXT("TargetSeat"), Seat);
	}
}

void ANPCAIController::InitBBKeys()
{
	auto* pBlackboardComponent = GetBlackboardComponent();
	if (!pBlackboardComponent) return;

	ABus* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));

	if (Bus)
	{
		auto checkLocation = Bus->GetCheckLocation();
		pBlackboardComponent->SetValueAsVector(TEXT("CheckLocation"), checkLocation);

		auto exitLocation = Bus->GetExitLocation();
		pBlackboardComponent->SetValueAsVector(TEXT("ExitLocation"), exitLocation);
		auto despawnLocation = Bus->GetDespawnLocation();
		pBlackboardComponent->SetValueAsVector(TEXT("DespawnLocation"), despawnLocation);

		auto seat = Bus->GetAvailableSeat();
		if (seat)
		{
			pBlackboardComponent->SetValueAsObject(TEXT("TargetSeat"), seat);
		}
	}
}
