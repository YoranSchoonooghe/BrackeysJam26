#include "IDActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"

AIDActor::AIDActor()
{
	PrimaryActorTick.bCanEverTick = false;

	auto* pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = pRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	FirstNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("FirstName"));
	FirstNameText->SetupAttachment(RootComponent);

	LastNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LastName"));
	LastNameText->SetupAttachment(RootComponent);

	DateOfBirthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DateOfBirth"));
	DateOfBirthText->SetupAttachment(RootComponent);

	IDNumberText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IDNumber"));
	IDNumberText->SetupAttachment(RootComponent);

	PhotoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PhotoWidget"));
	PhotoWidget->SetupAttachment(RootComponent);

	FirstNameText->SetText(FText::FromString(TEXT("JOHN")));
	LastNameText->SetText(FText::FromString(TEXT("DOE")));
	DateOfBirthText->SetText(FText::FromString(TEXT("00/00/0000")));
	IDNumberText->SetText(FText::FromString(TEXT("0000-0000")));
}

void AIDActor::BeginPlay()
{
	Super::BeginPlay();
	

}

//void AIDActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AIDActor::UpdateID(const FPassengerData& PassportData)
{
	FirstNameText->SetText(FText::FromString(PassportData.FirstName));
	LastNameText->SetText(FText::FromString(PassportData.LastName));
	DateOfBirthText->SetText(FText::FromString(PassportData.DOB));
	IDNumberText->SetText(FText::FromString(PassportData.IDNumber));

	UpdatePhotoUI(PassportData.PassengerPhoto);
}