#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Move(const FVector2D& Value)
{
	if (!Controller) return;

	const FRotator Rotation = Controller->GetControlRotation();

	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Value.Y);
	AddMovementInput(RightDirection, Value.X);
}

void APlayerCharacter::Jump()
{
	Super::Jump();

}

//void APlayerCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}



