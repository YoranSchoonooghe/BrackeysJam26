// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../DataManager.h"
#include "MonitorWidget.generated.h"

class UWidgetSwitcher;
class UCharacterInformationWidget;

UENUM(BlueprintType)
enum class EMonitorScreen : uint8
{
	Empty,
	RequestPermission,
	CameraView,
	CharDetails
};

UCLASS()
class BRACKEYSJAM26_API UMonitorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Monitor")
	void ShowScreen(EMonitorScreen Screen);

	UFUNCTION(BlueprintCallable, Category = "Monitor")
	void UpdateCharacterInfo(const FPassengerRecord& Record);

	UFUNCTION(BlueprintImplementableEvent, Category = "Easter Eggs")
	void BP_OnMichaelJacksonAppeared();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> ScreenSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCharacterInformationWidget> CharacterInfoWidget;
};
