#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleProp.generated.h"

UCLASS()
class PROJECTU_API ADestructibleProp : public AActor
{
	GENERATED_BODY()
	
public:	
	ADestructibleProp();

	UPROPERTY(VisibleAnywhere, Category = "Destructible")
	class UBoxComponent* Coll;
	UPROPERTY(VisibleAnywhere, Category = "Destructible")
	class UDestructibleComponent* DM;
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual void BeginPlay() override;
};
