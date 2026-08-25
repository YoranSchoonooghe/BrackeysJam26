#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class ANPCCharacter;
class UWidgetInteractionComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxLookYaw{ 90.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxLookPitch{ 70.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractRange{ 250.0f };

public:	
	//virtual void Tick(float DeltaTime) override;

	void Move(const FVector2D& Value);
	virtual void Jump() override;
	void Look(const FVector2D& Value);
	void Interact();

	void SetTargetNPC(ANPCCharacter* NPCCharacter);
	ANPCCharacter* GetTargetNPC() const { return TargetNPC; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInputLocked(bool bLocked);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractHit(AActor* HitActor);

private:
	void ClampLookAngle();

	float InitialControlYaw{ 0.0f };
	float InitialControlPitch{ 0.0f };

	ANPCCharacter* TargetNPC;

	bool bInputLocked{ false };

};
