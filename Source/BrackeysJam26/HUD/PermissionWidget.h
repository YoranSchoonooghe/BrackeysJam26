#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PermissionWidget.generated.h"

UCLASS()
class BRACKEYSJAM26_API UPermissionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Permission")
	void AllowNPCOnBus();

	UFUNCTION(BlueprintCallable, Category = "Permission")
	void DenyNPCFromBus();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPassengerAllowed);
	UPROPERTY(BlueprintAssignable, Category = "Permission")
	FOnPassengerAllowed OnPassengerAllowed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPassengerDenied);
	UPROPERTY(BlueprintAssignable, Category = "Permission")
	FOnPassengerDenied OnPassengerDenied;
};
