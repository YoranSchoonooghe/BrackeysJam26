#include "ImposterSoundComponent.h"
#include "BrackeysJam26/Bus/Bus.h"

UImposterSoundComponent::UImposterSoundComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UImposterSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	TimeBeforeNextNoise = FMath::FRandRange(MinTimeBeforeNoise, MinTimeBeforeNoise + TimeDelayRange);
}


void UImposterSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;

	if (ElapsedTime >= TimeBeforeNextNoise)
	{
		ElapsedTime = 0.0f;
		TimeBeforeNextNoise = FMath::FRandRange(MinTimeBeforeNoise, MinTimeBeforeNoise + TimeDelayRange);

		auto nrOfImposters = GetNrOfImposters();
		if (nrOfImposters == 0) return;

		OnMakeNoise.Broadcast(CalculateVolumeFactor(nrOfImposters));
	}
}

int32 UImposterSoundComponent::GetNrOfImposters() const
{
	auto* Bus = Cast<ABus>(GetOwner());
	return Bus->GetNumberOfImposters();
}

float UImposterSoundComponent::CalculateVolumeFactor(int32 nrOfImposters) const
{
	auto* Bus = Cast<ABus>(GetOwner());
	auto imposters = Bus->GetNumberOfImposters();

	float volumeFactor{ (float)imposters / MaxImposters };
	volumeFactor = FMath::Clamp(volumeFactor, 0.0f, 1.0f);

	return volumeFactor;
}


