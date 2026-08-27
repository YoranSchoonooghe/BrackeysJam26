// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterInformationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UCharacterInformationWidget::UpdateCharacterInformation(const FPassengerData& PassengerData, const FTicketData& TicketData, UTexture2D* CharacterImage)
{
	if (IDText)
	{
		IDText->SetText(FText::FromString(PassengerData.IDNumber));
	}

	if (BirthText)
	{
		BirthText->SetText(FText::FromString(PassengerData.DOB));
	}

	if (lastName)
	{
		lastName->SetText(FText::FromString(PassengerData.FirstName));
	}

	if (Surname)
	{
		Surname->SetText(FText::FromString(PassengerData.LastName));
	}

	if (BoardedText)
	{
		BoardedText->SetText(FText::FromString(TicketData.BoardingStop));
	}

	if (RouteText)
	{
		RouteText->SetText(FText::FromString(TicketData.FinalStop));
	}

	if (CharImage && CharacterImage)
	{
		CharImage->SetBrushFromTexture(CharacterImage);
	}
}
