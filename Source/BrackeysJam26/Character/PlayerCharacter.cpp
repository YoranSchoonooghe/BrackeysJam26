#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "../InteractableInterface.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	FollowCamera->bUsePawnControlRotation = true;

	GetMesh()->SetOwnerNoSee(true);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (Controller)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();

		InitialControlYaw = ControlRotation.Yaw;
		InitialControlPitch = ControlRotation.Pitch;
	}
}

//void APlayerCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

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

void APlayerCharacter::Look(const FVector2D& Value)
{
	if (!Controller) return;

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(-Value.Y);

	ClampLookAngle();
}

void APlayerCharacter::Interact()
{
	if (!FollowCamera) return;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + (FollowCamera->GetForwardVector() * InteractRange);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	if (bHit && HitResult.GetActor())
	{
		if (HitResult.GetActor()->Implements<UInteractableInterface>())
		{
			IInteractableInterface::Execute_Interact(HitResult.GetActor());
		}
	}
}

void APlayerCharacter::SetTargetNPC(ANPCCharacter* NPCCharacter)
{
	if (!NPCCharacter || TargetNPC == NPCCharacter) return;

	TargetNPC = NPCCharacter;
}

void APlayerCharacter::ClampLookAngle()
{
	FRotator controlRotation = Controller->GetControlRotation();

	float relativeYaw = FMath::FindDeltaAngleDegrees(
		InitialControlYaw,
		controlRotation.Yaw
	);

	relativeYaw = FMath::Clamp(
		relativeYaw,
		-MaxLookYaw,
		MaxLookYaw
	);

	controlRotation.Yaw = InitialControlYaw + relativeYaw;

	float relativePitch = FMath::FindDeltaAngleDegrees(
		InitialControlPitch,
		controlRotation.Pitch
	);

	relativePitch = FMath::Clamp(
		relativePitch,
		-MaxLookPitch,
		MaxLookPitch
	);

	controlRotation.Pitch = InitialControlPitch + relativePitch;

	Controller->SetControlRotation(controlRotation);
}
