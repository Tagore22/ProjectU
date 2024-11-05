#pragma once

#include "ActionInterface.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class PROJECTU_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialNum = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ActivateInventory = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility InventoryVisible = ESlateVisibility::Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility ActionMenuVisible = ESlateVisibility::Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ActionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FInvenStruct> Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySlotClicked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* MenuAnim;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PotionCount;
};
