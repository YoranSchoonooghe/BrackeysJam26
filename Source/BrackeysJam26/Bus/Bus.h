#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../DataManager.h"
#include "Bus.generated.h"

class USplineComponent;
class UArrowComponent;
class ABusSeat;
class UBusQueueComponent;
class AIDActor;
class ATicketActor;

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
	TObjectPtr<UStaticMeshComponent> RoofMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USplineComponent> Spline;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArrowComponent> CheckLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArrowComponent> ExitLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UArrowComponent> DespawnLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBusQueueComponent> BusQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seats")
	TArray<ABusSeat*> Seats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger Docs")
	TObjectPtr<AIDActor> IDActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger Docs")
	TObjectPtr<ATicketActor> TicketActor;

public:	
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	USplineComponent* GetSpline() const { return Spline; }

	UFUNCTION()
	FVector GetCheckLocation() const;
	UFUNCTION()
	FVector GetExitLocation() const;
	UFUNCTION()
	FVector GetDespawnLocation() const;
	UFUNCTION()
	ABusSeat* GetAvailableSeat() const;

	UFUNCTION(BlueprintCallable, Category = "Roof")
	void SetRoofVisibility(bool visible);

	UFUNCTION(BlueprintCallable, Category = "Passenger Docs")
	void SetPassengerDocsVisibility(bool bVisible, bool bSendEvent = true);

	UFUNCTION(BlueprintCallable, Category = "Bus|Documents")
	void UpdatePassengerDocs(const FPassengerRecord& Record);

	const TArray<ABusSeat*>& GetSeats() const { return Seats; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBusOpenDoors);
	UPROPERTY(BlueprintAssignable, Category = "Bus")
	FOnBusOpenDoors OnOpenDoors;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBusCloseDoors);
	UPROPERTY(BlueprintAssignable, Category = "Bus")
	FOnBusCloseDoors OnCloseDoors;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDocsVisibilityChanged);
	UPROPERTY(BlueprintAssignable, Category = "Bus")
	FOnDocsVisibilityChanged OnDocsVisibilityChanged;

	UFUNCTION(BlueprintCallable, Category = "Bus")
	bool AreDoorsOpen() const { return bAreDoorsOpen; }

private:
	UFUNCTION()
	void OpenDoors();
	UFUNCTION()
	void CloseDoors();

	bool bAreDoorsOpen = false;
};
