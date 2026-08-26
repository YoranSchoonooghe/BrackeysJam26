#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrackeysJam26/InspectableInterface.h"
#include "TicketActor.generated.h"

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

public:	
	//virtual void Tick(float DeltaTime) override;

};
