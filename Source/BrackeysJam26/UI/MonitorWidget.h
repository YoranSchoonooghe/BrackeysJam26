// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonitorWidget.generated.h"

class UWidgetSwitcher;

UENUM(BlueprintType)
enum class EMonitorScreen : uint8
{
	Empty,
	RequestPermission
};

UCLASS()
class BRACKEYSJAM26_API UMonitorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Monitor")
	void ShowScreen(EMonitorScreen Screen);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> ScreenSwitcher;
};
