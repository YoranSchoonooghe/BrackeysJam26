#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "DefaultPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;

UCLASS()
class BRACKEYSJAM26_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADefaultPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> LookAction;

private:
	void Move(const FInputActionValue& Value);
	void Jump();
	void Look(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<APlayerCharacter> CachedPlayerCharacter;
};
