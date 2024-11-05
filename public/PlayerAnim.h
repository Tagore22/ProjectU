#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PUPlayer.h"
#include "PlayerAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheck);
DECLARE_MULTICAST_DELEGATE(FOnDodgeEnd);
DECLARE_MULTICAST_DELEGATE(FOnDamagedEnd);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnBackStabEnd);
DECLARE_MULTICAST_DELEGATE(FOnKnockBackStart);
DECLARE_MULTICAST_DELEGATE(FOnKnockBackSkillEnd);
DECLARE_MULTICAST_DELEGATE(FOnPotionEnd);
DECLARE_MULTICAST_DELEGATE(FOnHealEnd);
DECLARE_MULTICAST_DELEGATE(FOnAssertCheck);
DECLARE_MULTICAST_DELEGATE(FOnAssertEnd);
DECLARE_MULTICAST_DELEGATE(FOnSpawnGhostTail);

UCLASS()
class PROJECTU_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
		
public:
	UPlayerAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void AttackStop();

	FOnAttackHitCheck attackHitCheck;
	FOnDodgeEnd dodgeEnd;
	FOnDamagedEnd damagedEnd;
	FOnAttackEnd attackEnd;
	FOnBackStabEnd backStabEnd;
	FOnKnockBackStart knockBackStart;
	FOnKnockBackSkillEnd knockBackSkillEnd;
	FOnPotionEnd potionEnd;
	FOnHealEnd healEnd;
	FOnAssertCheck assertCheck;
	FOnAssertEnd assertEnd;
	FOnSpawnGhostTail spawnGhostTail;

	UPROPERTY(VisibleAnywhere, Category = "AttackMontage")
	UAnimMontage* playerMontage;
	UPROPERTY(VisibleAnywhere, Category = "PotionMontage")
	UAnimMontage* PotionMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerAnim")
	bool isLockOn = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState")
	EPlayerState mState = EPlayerState::Idle;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerAnim", Meta = (AllowPrivateAccess = true))
	float speed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerAnim", Meta = (AllowPrivateAccess = true))
	float direction = 0.0f;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	UFUNCTION()
	void AnimNotify_DodgeEnd();
	UFUNCTION()
	void AnimNotify_DamagedEnd();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_BackStabEnd();
	UFUNCTION()
	void AnimNotify_KnockBackStart();
	UFUNCTION()
	void AnimNotify_KnockBackSkillEnd();
	UFUNCTION()
	void AnimNotify_potionEnd();
	UFUNCTION()
	void AnimNotify_healEnd();
	UFUNCTION()
	void AnimNotify_assertCheck();
	UFUNCTION()
	void AnimNotify_assertEnd();
	UFUNCTION()
	void AnimNotify_spawnGhostTail();
};
