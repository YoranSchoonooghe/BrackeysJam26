#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InspectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRACKEYSJAM26_API UInspectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInspectionComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspection")
	float StartScale{ 0.1f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspection")
	float TargetScale{ 1.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspection")
	float AnimationDuration{ 0.5f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspection")
	float RotationSpeed{ 1.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspection|Blur")
	float Aperture{ 32.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspection|Blur")
	float DOFSensorWidth{ 300.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inspection|Blur")
	float DOFFocalDistance{ 40.0f };


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inspection")
	void StartInspecting(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Inspection")
	void StopInspecting();
	UFUNCTION(BlueprintCallable, Category = "Inspection")
	bool IsInspecting() const { return bIsInspecting; }

	void Rotate(const FVector2D& Value, const FVector& CameraRight, const FVector& CameraUp);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartedInspecting);
	UPROPERTY(BlueprintAssignable, Category = "Inspection")
	FOnStartedInspecting OnStartedInspecting;

private:
	void UpdateInspectAnimation(float DeltaTime);
	void BlurBackground();
	void ResetBackground();

	UPROPERTY()
	TObjectPtr<AActor> OriginalActor;
	UPROPERTY()
	TObjectPtr<AActor> InspectionActor;

	FTransform StartTransform;
	FTransform TargetTransform;

	bool bIsInspecting{ false };
	bool bIsInspectAnimationPlaying{ false };
	float ElapsedAnimationTime{ 0.0f };

};
