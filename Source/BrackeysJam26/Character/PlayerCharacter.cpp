#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"


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
