#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBase.h"
#include "EnemyBaseAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackStart);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnTelpoEnd);
DECLARE_MULTICAST_DELEGATE(FOnMeteorStart);
DECLARE_MULTICAST_DELEGATE(FOnMeteorEnd);
DECLARE_MULTICAST_DELEGATE(FOnSummonStart);
DECLARE_MULTICAST_DELEGATE(FOnSummonEnd);
DECLARE_MULTICAST_DELEGATE(FOnBoltStart);
DECLARE_MULTICAST_DELEGATE(FOnBoltEnd);

UCLASS()
class PROJECTU_API UEnemyBaseAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
	FOnAttackHit attackHit;
	FOnAttackStart attackStart;
	FOnAttackEnd attackEnd;
	FOnTelpoEnd telpoEnd;
	FOnMeteorStart meteorStart;
	FOnMeteorEnd meteorEnd;
	FOnSummonStart summonStart;
	FOnSummonEnd summonEnd;
	FOnBoltStart boltStart;
	FOnBoltEnd boltEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bAttackPlay = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	EEnemyState mState = EEnemyState::Idle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyAnim", Meta = (AllowPrivateAccess = true))
	float speed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyAnim", Meta = (AllowPrivateAccess = true))
	float direction = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	bool isTarget;

	class AEnemyBase* me;

	UFUNCTION()
	void AnimNotify_EnemyDie();
	UFUNCTION()
	void AnimNotify_DamagedEnd();
	UFUNCTION()
	void AnimNotify_AttackHit();
	UFUNCTION()
	void AnimNotify_AttackStart();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_KnockBackEnd();
	UFUNCTION()
	void AnimNotify_TelpoEnd();
	UFUNCTION()
	void AnimNotify_MeteorStart();
	UFUNCTION()
	void AnimNotify_MeteorEnd();
	UFUNCTION()
	void AnimNotify_SummonStart();
	UFUNCTION()
	void AnimNotify_SummonEnd();
	UFUNCTION()
	void AnimNotify_BoltStart();
	UFUNCTION()
	void AnimNotify_BoltEnd();
};
