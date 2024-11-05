#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class PROJECTU_API APortal : public AActor
{
	GENERATED_BODY()
public:	
	APortal();
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* coll;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UParticleSystemComponent* VFX;
protected:
	virtual void BeginPlay() override;
};
