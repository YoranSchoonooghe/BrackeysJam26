// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonitorWidget.h"
#include "MonitorActor.generated.h"

class UWidgetComponent;

UCLASS()
class BRACKEYSJAM26_API AMonitorActor : public AActor
{
	GENERATED_BODY()

public:
	AMonitorActor();

	UFUNCTION(BlueprintCallable, Category = "Monitor")
	void ShowScreen(EMonitorScreen Screen);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> ScreenWidgetComponent;
};
