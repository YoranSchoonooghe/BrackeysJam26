#include "NPCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrackeysJam26/Components/SplineFollowComponent.h"

ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	

}

void ANPCCharacter::ChangeState(ENPCState NewState)
{
	if (NPCState == NewState) return;

	NPCState = NewState;
}

//void ANPCCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

