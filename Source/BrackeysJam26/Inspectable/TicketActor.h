#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrackeysJam26/InspectableInterface.h"
#include "../DataManager.h"
#include "TicketActor.generated.h"

class UTextRenderComponent;

UCLASS()
class BRACKEYSJAM26_API ATicketActor : public AActor, public IInspectableInterface
{
	GENERATED_BODY()
	
public:	
	ATicketActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> BoardingStopText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> FinalStopText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> DateText;

public:	
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Ticket")
	void UpdateTicket(const FTicketData& TicketData);

};
