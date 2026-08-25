#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BrackeysJam26/UI/MonitorActor.h"
#include "DefaultPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;
class ANPCCharacter;

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

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PermissionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> CloseButtonWidgetClass;

public:
	void ShowPermissionWidget(ANPCCharacter* NPC);
	void HidePermissionWidget();

	void ShowCloseButtonWidget();
	void HideCloseButtonWidget();

	UFUNCTION(BlueprintCallable, Category = "Monitor")
	void ShowMonitorScreen(EMonitorScreen Screen);

private:
	void Move(const FInputActionValue& Value);
	void Jump();
	void Look(const FInputActionValue& Value);
	void Interact();

	UPROPERTY()
	TObjectPtr<APlayerCharacter> CachedPlayerCharacter;

	UPROPERTY()
	TObjectPtr<UUserWidget> PermissionWidgetInstance;

	UPROPERTY()
	TObjectPtr<UUserWidget> CloseButtonWidgetInstance;

	UPROPERTY()
	TObjectPtr<AMonitorActor> CachedMonitor;
};
