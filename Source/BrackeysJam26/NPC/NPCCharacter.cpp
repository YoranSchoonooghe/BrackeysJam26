#include "NPCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrackeysJam26/Components/SplineFollowComponent.h"
#include "Components/CapsuleComponent.h"
#include "BrackeysJam26/Bus/Bus.h"
#include "BrackeysJam26/Bus/BusSeat.h"
#include "BrackeysJam26/Components/BusQueueComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	

}

//void ANPCCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ANPCCharacter::ChangeState(ENPCState NewState)
{
	if (NPCState == NewState) return;

	NPCState = NewState;

	if (NPCState == ENPCState::WalkToSeat)
	{
		AssignSeat();
	}

	if (NPCState == ENPCState::ExitBus || NPCState == ENPCState::WalkToSeat)
	{
		auto* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
		if (auto* Queue = Bus->GetComponentByClass<UBusQueueComponent>())
		{
			Queue->StartNextPassenger();
		}
	}
}

void ANPCCharacter::AssignSeat()
{
	auto* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
	if (!Bus) return;

	auto* Seat = Bus->GetAvailableSeat();
	if (!Seat) return;

	if (auto* AIController = Cast<AAIController>(GetController()))
	{
		if (auto* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetSeat"), Seat);
		}
	}
}

void ANPCCharacter::Eject(float Force)
{
	if (NPCState != ENPCState::Sitting) return;

	if (CurrentSeat)
	{
		CurrentSeat->Leave();
		CurrentSeat = nullptr;
	}

	const FVector launchDirection = FMath::VRandCone(FVector::UpVector, FMath::DegreesToRadians(20.0f));

	GetCharacterMovement()->DisableMovement();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetSimulatePhysics(true);

		MeshComp->AddImpulse(launchDirection * Force, NAME_None, true);

		MeshComp->WakeAllRigidBodies();
	}
}