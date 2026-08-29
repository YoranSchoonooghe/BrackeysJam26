#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ImposterSoundComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRACKEYSJAM26_API UImposterSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UImposterSoundComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imposter Sound")
	float MinTimeBeforeNoise{ 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imposter Sound")
	float TimeDelayRange{ 5.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imposter Sound")
	int32 MaxImposters{ 5 };

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMakeNoise, float, VolumeFactor);
	UPROPERTY(BlueprintAssignable, Category = "Imposter Sound")
	FOnMakeNoise OnMakeNoise;

private:
	int32 GetNrOfImposters() const;
	float CalculateVolumeFactor(int32 nrOfImposters) const;

	float ElapsedTime{ 0.0f };
	float TimeBeforeNextNoise{ 0.0f };
};
