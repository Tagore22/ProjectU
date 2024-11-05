#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class PROJECTU_API AArrow : public AActor
{
	GENERATED_BODY()
public:	
	AArrow();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		 const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UCapsuleComponent* coll;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere, Category = "Clang Sound")
	USoundBase* ClangSound;
	float speed = 1500.0f;
protected:
	virtual void BeginPlay() override;
	int32 Damage = 10;
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* Trail;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	class UParticleSystem* TrailRef;
	FVector SpawnLoc;
	const float DistanceLimit = 5000.0f;
};
