#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bus.generated.h"

class USplineComponent;
class UArrowComponent;
class ABusSeat;
class UBusQueueComponent;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArrowComponent> CheckLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArrowComponent> ExitLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBusQueueComponent> BusQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seats")
	TArray<ABusSeat*> Seats;

public:	
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	USplineComponent* GetSpline() const { return Spline; }

	UFUNCTION()
	FVector GetCheckLocation() const;
	UFUNCTION()
	FVector GetExitLocation() const;
	UFUNCTION()
	ABusSeat* GetAvailableSeat() const;

	const TArray<ABusSeat*>& GetSeats() const { return Seats; }
};
