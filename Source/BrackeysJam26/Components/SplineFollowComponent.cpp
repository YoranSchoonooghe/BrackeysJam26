#include "SplineFollowComponent.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SplineComponent.h"

USplineFollowComponent::USplineFollowComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USplineFollowComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwner = Cast<ANPCCharacter>(GetOwner());
}


void USplineFollowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsMoving)
    {
        UpdateSplineMovement(DeltaTime);
    }
}

void USplineFollowComponent::SetSpline(USplineComponent* Spline)
{
	if (!Spline) return;

	TargetSpline = Spline;

	bIsMoving = true;
	CurrentSplineDistance = 0.0f;
}

void USplineFollowComponent::UpdateSplineMovement(float DeltaTime)
{
	if (!TargetSpline) return;

    auto WalkSpeed = CachedOwner->GetCharacterMovement()->MaxWalkSpeed;

    float TotalSplineLength = TargetSpline->GetSplineLength();

    if (CurrentSplineDistance >= TotalSplineLength)
    {
        CachedOwner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
        bIsMoving = false;
        return;
    }

    CurrentSplineDistance += WalkSpeed * DeltaTime;

    CurrentSplineDistance = FMath::Clamp(CurrentSplineDistance, 0.0f, TotalSplineLength);

    FVector NewLocation = TargetSpline->GetLocationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World);
    FRotator NewRotation = TargetSpline->GetRotationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World);
    NewRotation.Pitch = 0.0f;

    CachedOwner->SetActorLocationAndRotation(NewLocation, NewRotation, false, nullptr, ETeleportType::TeleportPhysics);

    CachedOwner->GetCharacterMovement()->Velocity = NewRotation.Vector() * WalkSpeed;

    if (CurrentSplineDistance >= TotalSplineLength)
    {
        CachedOwner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
        bIsMoving = false;
    }
}

