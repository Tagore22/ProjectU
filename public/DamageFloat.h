#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageFloat.generated.h"

UCLASS()
class PROJECTU_API UDamageFloat : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageDone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;
};
