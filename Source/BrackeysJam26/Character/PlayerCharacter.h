#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BRACKEYSJAM26_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;

	void Move(const FVector2D& Value);
	virtual void Jump() override;

};
