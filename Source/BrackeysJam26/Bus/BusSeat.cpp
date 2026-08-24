#include "BusSeat.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"

ABusSeat::ABusSeat()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABusSeat::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void ABusSeat::Leave()
{
	if (!IsOccupied()) return;

	//Occupant->Leave()
	Occupant = nullptr;
}

void ABusSeat::Eject()
{
	if (!IsOccupied()) return;

	Occupant->Eject(EjectionForce);
	Occupant = nullptr;
}

bool ABusSeat::IsOccupied() const
{
	return (Occupant != nullptr);
}