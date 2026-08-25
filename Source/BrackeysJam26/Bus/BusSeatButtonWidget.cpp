// Fill out your copyright notice in the Description page of Project Settings.

#include "BusSeatButtonWidget.h"

void UBusSeatButtonWidget::PressButton()
{
	OnPressed.Broadcast();
}
