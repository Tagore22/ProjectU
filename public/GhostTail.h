#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostTail.generated.h"

UCLASS()
class PROJECTU_API AGhostTail : public AActor
{
	GENERATED_BODY()
public:	
	AGhostTail();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	void MakeGhostTail(USkeletalMeshComponent* Pawn);
private:
	UPROPERTY()
	class UPoseableMeshComponent* PoseableMesh;
	UPROPERTY()
	class UMaterialInstance* GhostMaterial;
	UPROPERTY()
	TArray<class UMaterialInstanceDynamic*> Materials;
	float FadeCountDown;
	float FadeOutTime = 0.5f;
	bool IsSpawned = false;
};
