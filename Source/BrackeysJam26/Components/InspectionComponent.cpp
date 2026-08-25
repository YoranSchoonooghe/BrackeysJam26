#include "InspectionComponent.h"
#include "BrackeysJam26/Character/PlayerCharacter.h"

UInspectionComponent::UInspectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UInspectionComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UInspectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsInspectAnimationPlaying)
        UpdateInspectAnimation(DeltaTime);
}

void UInspectionComponent::StartInspecting(AActor* Actor)
{
    if (!Actor) return;

    OriginalActor = Actor;

    auto* World = GetWorld();
    if (!World) return;

    auto player = Cast<APlayerCharacter>(GetOwner());
    if (!player) return;

    auto inspectionTransform = player->GetInspectionAnchor();
    inspectionTransform.SetScale3D(FVector(StartScale, StartScale, StartScale));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    InspectionActor = World->SpawnActor<AActor>(
        Actor->GetClass(),
        inspectionTransform,
        SpawnParams
    );

    if (!InspectionActor) return;

    OriginalActor->SetActorHiddenInGame(true);
    StartTransform = inspectionTransform;
    TargetTransform = StartTransform;
    TargetTransform.SetScale3D(FVector(TargetScale, TargetScale, TargetScale));

    bIsInspecting = true;
    ElapsedAnimationTime = 0.0f;
    bIsInspectAnimationPlaying = true;
}

void UInspectionComponent::StopInspecting()
{
    auto* player = Cast<APlayerCharacter>(GetOwner());
    if (!player) return;

    OriginalActor->SetActorHiddenInGame(false);
    InspectionActor->Destroy();
    bIsInspecting = false;

    player->SetInputLocked(false);
}

void UInspectionComponent::Rotate(const FVector2D& Value, const FVector& CameraRight, const FVector& CameraUp)
{
    FQuat PitchQuat(CameraRight, FMath::DegreesToRadians(Value.Y * RotationSpeed));
    FQuat YawQuat(CameraUp, FMath::DegreesToRadians(-Value.X * RotationSpeed));

    FQuat CombinedRotation = YawQuat * PitchQuat;

    InspectionActor->AddActorWorldRotation(CombinedRotation);
}

void UInspectionComponent::UpdateInspectAnimation(float DeltaTime)
{
    ElapsedAnimationTime += DeltaTime;

    float alpha = ElapsedAnimationTime / AnimationDuration;

    alpha = FMath::Clamp(alpha, 0.0f, 1.0f);
    alpha = FMath::InterpEaseInOut(0.0f, 1.0f, alpha, 2.0f);

    FTransform NewTransform;

    NewTransform.Blend(
        StartTransform,
        TargetTransform,
        alpha
    );

    const float CurrentAngleDegrees = alpha * 360.0f;
    const FQuat LocalRotation = FQuat(FVector::UpVector, FMath::DegreesToRadians(CurrentAngleDegrees));
    NewTransform.SetRotation(NewTransform.GetRotation() * LocalRotation);

    InspectionActor->SetActorTransform(NewTransform);

    if (alpha >= 1.0f)
    {
        InspectionActor->SetActorTransform(TargetTransform);
        bIsInspectAnimationPlaying = false;
    }
}

