#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bus.generated.h"

class USplineComponent;

UCLASS()
class BRACKEYSJAM26_API ABus : public AActor
{
	GENERATED_BODY()
	
public:	
	ABus();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USplineComponent> Spline;

public:	
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	USplineComponent* GetSpline() const { return Spline; }
};
