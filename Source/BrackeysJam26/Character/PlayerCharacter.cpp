#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EngineBaseTypes.h"
#include "../InteractableInterface.h"
#include "../InspectableInterface.h"
#include "BrackeysJam26/NPC/NPCCharacter.h"
#include "BrackeysJam26/Character/DefaultPlayerController.h"
#include "Components/ArrowComponent.h"
#include "BrackeysJam26/Components/InspectionComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "BrackeysJam26/Bus/Bus.h"
#include "BrackeysJam26/Bus/BusRouteManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	FollowCamera->bUsePawnControlRotation = true;

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(FollowCamera);
	WidgetInteraction->InteractionSource = EWidgetInteractionSource::CenterScreen;

	GetMesh()->SetOwnerNoSee(true);

	InspectionAnchor = CreateDefaultSubobject<UArrowComponent>(TEXT("InspectionAnchor"));
	InspectionAnchor->SetupAttachment(FollowCamera);
	InspectionAnchor->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));

	Inspection = CreateDefaultSubobject<UInspectionComponent>(TEXT("Inspection"));
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

	if (WidgetInteraction)
	{
		WidgetInteraction->InteractionDistance = InteractRange;
	}

	if (AActor* BusActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABus::StaticClass()))
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
		AttachToActor(BusActor, AttachRules);

		if (ABusRouteManager* RouteManager = Cast<ABusRouteManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(), ABusRouteManager::StaticClass())))
		{
			RouteManager->OnDepart.AddDynamic(this, &APlayerCharacter::OnBusDepart);
			RouteManager->OnArrive.AddDynamic(this, &APlayerCharacter::OnBusArrive);
		}
	}

	//if (HUDWidget)
	//{
	//	HUDWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);

	//	if (HUDWidgetInstance)
	//	{
	//		HUDWidgetInstance->AddToViewport();

	//		HUDWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInputLocked)
	{
		UpdateLockedInteractionTrace();
		UpdateHoveredNPC();
		return;
	}

	if (!FollowCamera || bIsLoading)
	{
		OnUpdateInteractionPrompt(FText::GetEmpty());
		return;
	}

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
			FText Prompt = IInteractableInterface::Execute_GetPromptText(HitResult.GetActor());
			OnUpdateInteractionPrompt(Prompt);
		}
		else if (HitResult.GetActor()->Implements<UInspectableInterface>())
		{
			OnUpdateInteractionPrompt(FText::FromString("Click to Inspect"));
		}
		else
		{
			OnUpdateInteractionPrompt(FText::GetEmpty());
		}
	}
	else
	{
		OnUpdateInteractionPrompt(FText::GetEmpty());
	}
}

void APlayerCharacter::Move(const FVector2D& Value)
{
	if (!Controller || bInputLocked) return;

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
	if (!Controller || bInputLocked) return;

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(-Value.Y);

	ClampLookAngle();
}

void APlayerCharacter::Interact()
{
	if (bIsLoading) return;

	if (bInputLocked)
	{
		if (!WidgetInteraction) return;

		if (Cast<UWidgetComponent>(LockedInteractionHit.GetComponent()))
		{
			WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
			WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
			return;
		}

		AActor* HitActor = LockedInteractionHit.GetActor();
		if (HitActor && HitActor->Implements<UInteractableInterface>())
		{
			IInteractableInterface::Execute_Interact(HitActor);
		}

		return;
	}

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
		else if (HitResult.GetActor()->Implements<UInspectableInterface>())
		{
			if (Inspection->IsInspecting()) return;

			Inspection->StartInspecting(HitResult.GetActor());
			SetInputLocked(true);
		}
	}
}

void APlayerCharacter::RotateItem(const FVector2D& Value)
{
	if (!bInputLocked || !Inspection) return;
	if (!Inspection->IsInspecting()) return;

	Inspection->Rotate(Value, FollowCamera->GetRightVector(), FollowCamera->GetUpVector());
}

void APlayerCharacter::SetTargetNPC(ANPCCharacter* NPCCharacter)
{
	if (!NPCCharacter || TargetNPC == NPCCharacter) return;

	TargetNPC = NPCCharacter;
}

void APlayerCharacter::SetInputLocked(bool bLocked)
{
	bInputLocked = bLocked;

	if (auto* PC = Cast<ADefaultPlayerController>(Controller))
	{
		PC->SetShowMouseCursor(bLocked);

		if (bLocked)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false);
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);

			PC->ShowCloseButtonWidget();
		}
		else
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->HideCloseButtonWidget();
		}
	}

	if (WidgetInteraction)
	{
		WidgetInteraction->InteractionSource = bLocked ? EWidgetInteractionSource::Custom : EWidgetInteractionSource::CenterScreen;
		WidgetInteraction->InteractionDistance = bLocked ? FocusedInteractionDistance : InteractRange;
	}

	if (!bIsLoading)
	{
		ToggleHUDVisibility(!bLocked);
	}

	if (!bLocked && HoveredNPC)
	{
		HoveredNPC->SetHighlighted(false);
		HoveredNPC = nullptr;
	}
}

FTransform APlayerCharacter::GetInspectionAnchor() const
{
	return InspectionAnchor->GetComponentTransform();
}

void APlayerCharacter::SetLoadingState(bool bNewLoadingState)
{
	bIsLoading = bNewLoadingState;

	ToggleHUDVisibility(!bIsLoading);
}

void APlayerCharacter::ToggleHUDVisibility(bool bIsVisible)
{
	if(!HUDWidgetInstance)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);

		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
	

	if(HUDWidgetInstance)
	{
		if (bIsVisible)
		{
			HUDWidgetInstance->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			HUDWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APlayerCharacter::UpdateLockedInteractionTrace()
{
	LockedInteractionHit = FHitResult();

	if (!WidgetInteraction) return;

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	FVector WorldLocation, WorldDirection;
	if (!PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) return;

	const FVector TraceEnd = WorldLocation + WorldDirection * WidgetInteraction->InteractionDistance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(LockedInteractionHit, WorldLocation, TraceEnd, ECC_Visibility, CollisionParams);

	WidgetInteraction->SetCustomHitResult(LockedInteractionHit);
}

void APlayerCharacter::UpdateHoveredNPC()
{
	auto* NPC = Cast<ANPCCharacter>(LockedInteractionHit.GetActor());
	if (NPC && !NPC->IsSitting())
	{
		NPC = nullptr;
	}

	if (NPC == HoveredNPC) return;

	if (HoveredNPC)
	{
		HoveredNPC->SetHighlighted(false);
	}

	HoveredNPC = NPC;

	if (HoveredNPC)
	{
		HoveredNPC->SetHighlighted(true);
	}
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

void APlayerCharacter::OnBusDepart()
{
	if (UCharacterMovementComponent* CMC = GetCharacterMovement())
	{
		CMC->StopMovementImmediately();
		CMC->SetMovementMode(MOVE_None);
	}
}

void APlayerCharacter::OnBusArrive()
{
	if (UCharacterMovementComponent* CMC = GetCharacterMovement())
	{
		CMC->SetMovementMode(MOVE_Walking);
	}
}