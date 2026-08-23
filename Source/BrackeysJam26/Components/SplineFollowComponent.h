#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplineFollowComponent.generated.h"

class USplineComponent;
class ANPCCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRACKEYSJAM26_API USplineFollowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USplineFollowComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void SetSpline(USplineComponent* Spline);

private:
	void UpdateSplineMovement(float DeltaTime);

	USplineComponent* TargetSpline;
	bool bIsMoving{ false };
	float CurrentSplineDistance{ 0.0f };

	ANPCCharacter* CachedOwner;
};
