#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBar.generated.h"

UCLASS()
class PROJECTU_API UEnemyHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	float HPRatio;
	UPROPERTY(BlueprintReadOnly)
	float Damaged = 0.0f;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* DamageText;
};
