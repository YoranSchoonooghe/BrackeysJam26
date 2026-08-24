// Fill out your copyright notice in the Description page of Project Settings.

#include "MonitorWidget.h"
#include "Components/WidgetSwitcher.h"

void UMonitorWidget::ShowScreen(EMonitorScreen Screen)
{
	if (ScreenSwitcher)
	{
		ScreenSwitcher->SetActiveWidgetIndex(static_cast<int32>(Screen));
	}
}
