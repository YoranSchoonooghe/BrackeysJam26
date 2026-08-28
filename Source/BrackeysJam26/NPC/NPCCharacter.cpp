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
#include "BrackeysJam26/Components/ExpireComponent.h"

ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Expire = CreateDefaultSubobject<UExpireComponent>(TEXT("Expire"));
}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RandomizeRaceAndVisuals();
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

		if (Bus)
		{
			Bus->SetPassengerDocsVisibility(false);
		}
	}

	if (NPCState == ENPCState::Sitting)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetCharacterMovement()->DisableMovement();

		if (CurrentSeat)
		{
			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, false);
			AttachToActor(CurrentSeat, AttachRules);
		}
	}
}

void ANPCCharacter::Despawn()
{
	Destroy();
}

void ANPCCharacter::Interact_Implementation()
{
	if (NPCState != ENPCState::Sitting || !CurrentSeat) return;

	const bool bWasVisible = CurrentSeat->IsButtonVisible();

	if (auto* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass())))
	{
		for (ABusSeat* Seat : Bus->GetSeats())
		{
			if (Seat)
			{
				Seat->SetButtonVisible(false);
			}
		}
	}

	CurrentSeat->SetButtonVisible(!bWasVisible);
}

void ANPCCharacter::SetHighlighted(bool bHighlighted)
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetOverlayMaterial(bHighlighted ? OutlineMaterial : nullptr);
	}
}

void ANPCCharacter::RandomizeRaceAndVisuals()
{
	if (RaceVisuals.IsEmpty()) return;

	TArray<ENPCRace> AvailableRaces;
	RaceVisuals.GetKeys(AvailableRaces);

	int32 RandomIndex = FMath::RandRange(0, AvailableRaces.Num() - 1);
	ENPCRace SelectedRace = AvailableRaces[RandomIndex];
	FNPCModelData SelectedData = RaceVisuals[SelectedRace];

	if (SelectedData.RaceMesh)
	{
		GetMesh()->SetSkeletalMesh(SelectedData.RaceMesh);
	}

	if (!SelectedData.FaceVariations.IsEmpty())
	{
		int32 RandomFace = FMath::RandRange(0, SelectedData.FaceVariations.Num() - 1);
		FFaceVariation SelectedFace = SelectedData.FaceVariations[RandomFace];

		if (SelectedFace.FaceMaterial)
		{
			GetMesh()->SetMaterial(1, SelectedFace.FaceMaterial);
		}

		if (SelectedFace.IDPhoto)
		{
			PassengerRecord.PresentedPassport.PassengerPhoto = SelectedFace.IDPhoto;
		}
	}
}

void ANPCCharacter::AssignSeat()
{
	auto* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
	if (!Bus) return;

	auto* Seat = Bus->GetAvailableSeat();
	if (!Seat) return;

	Seat->Occupy(this);

	if (auto* AIController = Cast<AAIController>(GetController()))
	{
		if (auto* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetSeat"), Seat);
		}
	}
}

bool ANPCCharacter::Eject(float Force)
{
	if (NPCState != ENPCState::Sitting) return false;

	if (CurrentSeat)
	{
		CurrentSeat->Leave();
		CurrentSeat = nullptr;
	}

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(DetachRules);

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

	Expire->StartExpire();

	return true;
}