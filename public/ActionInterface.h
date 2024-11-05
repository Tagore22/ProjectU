#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UActionInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTU_API IActionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseItem();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropItem(class AActor* ItemToDrop);
};

USTRUCT(Atomic, BlueprintType)
struct FInvenStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* ItemImage;
	// �������� �ֿﶧ �ߴ� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText PickupText; 
	// �κ��丮â���� ��ư�� �ߴ� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText ActionText;
};
