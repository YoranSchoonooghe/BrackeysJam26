// Fill out your copyright notice in the Description page of Project Settings.

#include "MonitorWidget.h"
#include "Components/WidgetSwitcher.h"
#include "../HUD/CharacterInformationWidget.h"

void UMonitorWidget::ShowScreen(EMonitorScreen Screen)
{
	//if (ScreenSwitcher)
	//{
	//	ScreenSwitcher->SetActiveWidgetIndex(static_cast<int32>(Screen));
	//}

	//if (CharacterInfoWidget)
	//{
	//	CharacterInfoWidget->SetVisibility(Screen == EMonitorScreen::RequestPermission ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	//}
}

void UMonitorWidget::UpdateCharacterInfo(const FPassengerRecord& Record)
{
	if (CharacterInfoWidget)
	{
		CharacterInfoWidget->UpdateCharacterInformation(Record.TrueIdentity, Record.TrueTicket, Record.TrueIdentity.PassengerPhoto);
	}
}
