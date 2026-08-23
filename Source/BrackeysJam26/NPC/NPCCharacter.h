#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

class USplineFollowComponent;

UCLASS()
class BRACKEYSJAM26_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USplineFollowComponent> SplineFollow;

public:
	//virtual void Tick(float DeltaTime) override;

private:
	float CurrentSplineDistance{ 0.0f };
};
