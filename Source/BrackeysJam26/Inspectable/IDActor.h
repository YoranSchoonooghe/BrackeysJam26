#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrackeysJam26/InspectableInterface.h"
#include "../DataManager.h"
#include "IDActor.generated.h"

class UTextRenderComponent;
class UWidgetComponent;
class UTexture2D;

UCLASS()
class BRACKEYSJAM26_API AIDActor : public AActor, public IInspectableInterface
{
	GENERATED_BODY()
	
public:	
	AIDActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> FirstNameText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> LastNameText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> DateOfBirthText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> IDNumberText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> PhotoWidget;

	UFUNCTION(BlueprintImplementableEvent, Category = "ID")
	void UpdatePhotoUI(UTexture2D* NewPhoto);

public:	
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ID")
	void UpdateID(const FPassengerData& PassportData);
};
