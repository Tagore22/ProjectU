#pragma once

#include "CoreMinimal.h"
#include "Arrow.h"
#include "Bolt.generated.h"

UCLASS()
class PROJECTU_API ABolt : public AArrow
{
	GENERATED_BODY()
public:
	ABolt();	
private:
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UParticleSystemComponent* BoltEffect;
	const int32 BOLTDAMAGE = 30;
};
