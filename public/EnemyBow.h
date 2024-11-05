#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBow.generated.h"

UCLASS()
class PROJECTU_API AEnemyBow : public AEnemyBase
{
	GENERATED_BODY()
public:
	AEnemyBow();
	virtual void Tick(float DeltaTime) override;
	void ShootTrace();
	UFUNCTION()
	void SpawnArrow() const;
	UFUNCTION(BlueprintCallable)
	void ResetSight();

	UPROPERTY(EditAnywhere, Category = "Arrow")
	TSubclassOf<class AArrow> arrow;
	UPROPERTY(BlueprintReadOnly, Category = "CanSeePlayer")
	bool canSee = false;
	UPROPERTY(BlueprintReadOnly, Category = "SightDelay")
	float sightDelay = 0.0f;
protected:
	virtual void BeginPlay() override;
	virtual void EnemyAttack() override;
private:
	// EnemyAX의 전용값들. 모두 생성자에서 부모클래스에 대응하는 값들로 바뀌어야한다.
	const float BOW_WALKSPEED = 150.0f;
	const float BOW_RUNSPEED = 600.0f;
	const int32 BOW_MINCOMBO = 1;
	const int32 BOW_MAXCOMBO = 1;
	const float BOW_TARGETSPEED = 75.0f;
	const float BOW_SIGHTDISTANCE = 5000.0f;
	const int32 BOW_DAMAGE = 12;
};
