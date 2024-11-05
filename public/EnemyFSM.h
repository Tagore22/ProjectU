/*#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTU_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
	EEnemyState mState = EEnemyState::Idle;
	UPROPERTY()
	class UEnemyBaseAnim* anim;

	void OnDamageProcess();

private:
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	FVector randomPos;
	bool GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& Dest);

	UPROPERTY()
	class APUPlayer* target;
	UPROPERTY()
	class AEnemyBase* me;

	UPROPERTY(EditDefaultsOnly, Category = "FSM", Meta = (AllowPrivateAccess = true))
	float idleDelayTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "FSM", Meta = (AllowPrivateAccess = true))
	float currentTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "FSM", Meta = (AllowPrivateAccess = true))
	float attackRange = 150.0f;
	UPROPERTY(EditDefaultsOnly, Category = "FSM", Meta = (AllowPrivateAccess = true))
	float attackDelayTime = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "FSM", Meta = (AllowPrivateAccess = true))
	float damageDelayTime = 0.4f;
	UPROPERTY(EditDefaultsOnly, Category = "FSM", Meta = (AllowPrivateAccess = true))
	float dieSpeed = 50.0f;
	UPROPERTY(EditDefaultsOnly, Category = "FSM", Meta = (AllowPrivateAccess = true))
	int32 hp = 3;
	UPROPERTY()
	class AAIController* ai;
};*/
