#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class ANPCCharacter;
class UWidgetInteractionComponent;
class UUserWidget;
class UInspectionComponent;

UCLASS()
class BRACKEYSJAM26_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteraction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArrowComponent> InspectionAnchor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UInspectionComponent> Inspection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxLookYaw{ 90.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxLookPitch{ 70.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractRange{ 250.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float FocusedInteractionDistance{ 5000.0f };

public:	
	virtual void Tick(float DeltaTime) override;

	void Move(const FVector2D& Value);
	virtual void Jump() override;
	void Look(const FVector2D& Value);
	void Interact();
	void RotateItem(const FVector2D& Value);

	void SetTargetNPC(ANPCCharacter* NPCCharacter);
	ANPCCharacter* GetTargetNPC() const { return TargetNPC; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInputLocked(bool bLocked);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractHit(AActor* HitActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnUpdateInteractionPrompt(const FText& PromptMessage);

	FTransform GetInspectionAnchor() const;
	UCameraComponent* GetCamera() const { return FollowCamera; }

	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bIsLoading = false;

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetLoadingState(bool bNewLoadingState);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidget;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleHUDVisibility(bool bIsVisible);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> HUDWidgetInstance;

private:
	void ClampLookAngle();
	void UpdateHoveredNPC();
	void UpdateLockedInteractionTrace();

	float InitialControlYaw{ 0.0f };
	float InitialControlPitch{ 0.0f };

	ANPCCharacter* TargetNPC;

	UPROPERTY()
	TObjectPtr<ANPCCharacter> HoveredNPC;

	FHitResult LockedInteractionHit;

	bool bInputLocked{ false };

};
