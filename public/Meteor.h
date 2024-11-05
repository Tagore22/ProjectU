#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteor.generated.h"

UCLASS()
class PROJECTU_API AMeteor : public AActor
{
	GENERATED_BODY()
public:	
	AMeteor();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* coll;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraSystem* VFXSystem;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraComponent* VFXComponent;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraSystem* MagmaVFXSystem;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraComponent* MagmaVFXComponent;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraSystem* EarthVFXSystem;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraComponent* EarthVFXComponent;
	UPROPERTY()
	TSubclassOf<class UCameraShakeBase> CameraShake;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class USoundBase* MeteorSound;

	const int METEORDAMAGE = 70;
	const float METEORSPEED = 350.0f;
};
