#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickText.generated.h"

UCLASS()
class PROJECTU_API UPickText : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PickupText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PickupActor;
};
