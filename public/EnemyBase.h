#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Run,
	Die,
	SuperArmor
};

UCLASS()
class PROJECTU_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();
    virtual void Tick(float DeltaTime) override;
	virtual void OnDamageProcess(int32 Damage, bool IsKnockBack);

	UFUNCTION(BlueprintCallable)
	void BeginSprint();
	UFUNCTION(BlueprintCallable)
	void EndSprint();
	UFUNCTION(BlueprintCallable)
	void BeginTarget();
	UFUNCTION(BlueprintCallable)
	void EndTarget();
	UFUNCTION(BlueprintCallable)
	float PlayAttackMontage();
	void VisibleHPBar();
	void TimerHPBar();
	void TimerDamageFloat();
	void LockedOn(bool IsLocked);
	void Spawned();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float patrolWaitDuration = 1.0f;
	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<class AActor*> patrolArr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	EEnemyState mState = EEnemyState::Idle;
	UPROPERTY()
	class APUPlayer* target;
	UPROPERTY(BlueprintReadOnly)
	bool isTarget = false;
	UPROPERTY(BlueprintReadOnly)
	FVector homeLocation;
	bool isDieEnd = false;
	bool isAlive = true;
	bool isTargeted = false;
	UPROPERTY(BlueprintReadOnly)
	bool isSpawned = false;
	FVector f_Vec;
	FVector r_Vec;
	UPROPERTY(BlueprintReadOnly)
	int32 hp = 1000; // ü�� ��ġ��. ���߿� �����Ҽ� ����.
	float InitHP = 1000.0f;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	UPROPERTY(VisibleAnywhere, Category = "EnemyMontage")
	UAnimMontage* montage;
	UPROPERTY()
	class UEnemyBaseAnim* anim;
	// Enemy�� �� �⺻����. �Ŀ� �ڽ�Ŭ�������� const�� �ʱ�ȭ�� ��ü�������� �����ڿ����� ���� �ٲ��.
	UPROPERTY(EditAnywhere, Category = "MoveSpeed")
	float walkSpeed = 150.0f;
	UPROPERTY(EditAnywhere, Category = "MoveSpeed")
	float runSpeed = 600.0f;
	UPROPERTY(EditAnywhere, Category = "TargetSpeed")
	float targetSpeed = 75.0f;
	UPROPERTY(EditAnywhere, Category = "AttackRadius")
	float attackRadius = 50.0f;
	UPROPERTY(EditAnywhere, Category = "AttackRange")
	float attackRange = 200.0f;
	class UWidgetComponent* HPBar;
	class UEnemyHPBar* HPRef;
	class UWidgetComponent* LockWidget;
	class UUserWidget* LockRef;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDamageFloat> DamageHUD;
	class UDamageFloat* DamageRef;

	int32 minCombo;
	int32 maxCombo;
	int32 damage;
	FRotator rot;
	bool isBoss = false;

	// ���� �� Ÿ�Ժ��� �ٸ� �ʿ䰡 ������ ���� �׳� �ϳ��� ��ģ��.
	const float DIESPEED = 50.0f;
	const float ROTSPEED = 10.0f;
	const int32 MINDAMAGE = 1;
	const int32	MAXDAMAGE = 2;
	FTimerHandle DelayTimer;

	void PlayMontage(FName sectionName);
	void Die();
	UFUNCTION(BlueprintCallable)
	virtual void EnemyAttack();
	UFUNCTION()
	void LookTarget();
	void UpdateHP(int32 Damage);
};

