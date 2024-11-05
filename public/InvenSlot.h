#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvenSlot.generated.h"

UCLASS()
class PROJECTU_API UInvenSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* PickupImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySlot;
};
