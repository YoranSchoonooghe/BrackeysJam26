#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExpireComponent.generated.h"

enum class ExpirationState : uint8_t
{
	Disabled,
	Idle,
	Blinking
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRACKEYSJAM26_API UExpireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UExpireComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Expire")
	void StartExpire();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expire")
	float TimeBeforeBlink{ 7.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expire")
	float TimeBeforeExpire{ 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expire")
	float BlinkFrequency{ 0.1f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Expire")
	bool bDestroyOnExpired{ true };

private:
	ExpirationState State{ ExpirationState::Disabled };

	float ElapsedTime{ 0.0f };
	float ElapsedBlinkTime{ 0.0f };

	void UpdateIdleState(float DeltaTime);
	void UpdateBlinkingState(float DeltaTime);
	void ToggleVisibility();
};
