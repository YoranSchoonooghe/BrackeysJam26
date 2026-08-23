#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bus.generated.h"

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

public:	
	//virtual void Tick(float DeltaTime) override;

};
