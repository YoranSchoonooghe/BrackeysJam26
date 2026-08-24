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
};
