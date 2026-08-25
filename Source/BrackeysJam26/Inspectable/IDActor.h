#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrackeysJam26/InspectableInterface.h"
#include "IDActor.generated.h"

UCLASS()
class BRACKEYSJAM26_API AIDActor : public AActor, public IInspectableInterface
{
	GENERATED_BODY()
	
public:	
	AIDActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
