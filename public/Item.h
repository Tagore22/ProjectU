#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJECTU_API UItem : public UObject
{
	GENERATED_BODY()
public:
	UItem();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class APUPlayer* player);
	virtual void Use(class APUPlayer* player);
	virtual class UWorld* GetWorld() const;

	UPROPERTY(Transient)
	class UWorld* world;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText useActionText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* pickupMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UTexture2D* thumbnail;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText itemDisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText itemDescription;
	UPROPERTY()
	class UInventoryComponent* owningInventory;
};
