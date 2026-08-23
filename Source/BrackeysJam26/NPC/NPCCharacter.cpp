#include "NPCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrackeysJam26/Components/SplineFollowComponent.h"
#include "BrackeysJam26/Bus/Bus.h"
#include "Kismet/GameplayStatics.h"

ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    SplineFollow = CreateDefaultSubobject<USplineFollowComponent>(TEXT("SplineFollowComponent"));
}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->SetMovementMode(MOVE_Custom);

    ABus* Bus = Cast<ABus>(UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()));
    
    if (Bus)
    {
        auto* spline = Bus->GetSpline();
        SplineFollow->SetSpline(spline);
    }
}

//void ANPCCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

