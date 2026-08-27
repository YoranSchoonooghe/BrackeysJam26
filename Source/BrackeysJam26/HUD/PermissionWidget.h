#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PermissionWidget.generated.h"

class UButton;

UCLASS()
class BRACKEYSJAM26_API UPermissionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Permission")
	void AllowNPCOnBus();

	UFUNCTION(BlueprintCallable, Category = "Permission")
	void DenyNPCFromBus();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> AllowButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> DenyButton;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPassengerAllowed);
	UPROPERTY(BlueprintAssignable, Category = "Permission")
	FOnPassengerAllowed OnPassengerAllowed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPassengerDenied);
	UPROPERTY(BlueprintAssignable, Category = "Permission")
	FOnPassengerDenied OnPassengerDenied;
};
