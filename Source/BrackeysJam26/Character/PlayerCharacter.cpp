#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = -10.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, BaseEyeHeight);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetMesh()->SetOwnerNoSee(true);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
}
