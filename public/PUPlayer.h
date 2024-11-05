#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ActionInterface.h"
#include "PUPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,
	Run,
	Attack,
	Damage,
	Potion,
	Dodge,
	BackStab,
	KnockBackSkill,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickupItem);

UCLASS(BlueprintType)
class PROJECTU_API APUPlayer : public ACharacter, public IActionInterface
{
	GENERATED_BODY()

public:
	APUPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnHitEvent(int32 damage);
	void PlusHP(int32 Num);
	void ToggleInventory();
	virtual void DropItem_Implementation(class AActor* ItemToDrop) override;
	UPROPERTY(EditAnywhere)
	class USceneComponent* DropLoc;

	FVector f_Vec;
	FVector r_Vec;
	FVector curDir = FVector::ZeroVector;
	EPlayerState mState = EPlayerState::Idle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class UInventoryComponent* inventory;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPickupItem PickupItemDel;
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UMainWidget* HUDRef;
	UPROPERTY(VisibleAnywhere, Category = "AxeMesh")
	class USkeletalMeshComponent* AxeMesh;
	UPROPERTY(VisibleAnywhere, Category = "SwordMesh")
	class USkeletalMeshComponent* SwordMesh;
	UPROPERTY(VisibleAnywhere, Category = "MaceMesh")
	class USkeletalMeshComponent* MaceMesh;
	UPROPERTY(VisibleAnywhere, Category = "DaggerMesh")
	class USkeletalMeshComponent* DaggerMesh;
	UPROPERTY(VisibleAnywhere, Category = "PotionMesh")
	class UStaticMeshComponent* PotionMesh;
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponNum = 4;
	UPROPERTY(BlueprintReadOnly)
	bool PrevIsFood;
	float WalkSound_MaxRange = 300.0f;
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class UCameraComponent* camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP", Meta = (AllowPrivateAccess = true))
	int32 hp = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", Meta = (AllowPrivateAccess = true))
	float Mana = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina", Meta = (AllowPrivateAccess = true))
	float stamina = 100.0f;
	UPROPERTY(Meta = (AllowPrivateAccess = true))
	class UPlayerAnim* anim;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputMappingContext* mapContext;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* dodgeAction;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* runAction;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class USoundBase* hitSound;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraSystem* VFXSystem;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraComponent* VFXComponent;
	UPROPERTY()
	TSubclassOf<class UCameraShakeBase> CameraShake;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UParticleSystemComponent* PotionEffect;

	const float ATTACKRADIUS = 50.0f;
	const float ATTACKRANGE = 200.0f;
	const float WALKSPEED = 200.0f;
	const float RUNSPEED = 600.0f;
	const float LOCKONRADIUS = 1000.0f;
	const float LOCKONDIST = 20.0f;
	const float LOCKONRANGE = 1000.0f;
	const float CONSUME_STA = 10.0f;
	const float CONSUME_MANA = 10.0f; 
	const float CONSUME_ASSERT_MANA = 5.0f;
	const float RECOVERY_STA = 0.3f;
	const float MAX_STA = 100.0f;
	const float KNOCKBACK_RANGE = 400.0f;
	const float KNOCKBACK_FORCE = 200000.0f;
	const float MAX_FOV = 150.0f;
	const float MIN_FOV = 30.0f;
	const float MIN_ARM_LENGTH = 100.0f;
	const float MAX_ARM_LENGTH = 700.0f;
	const int32 KNOCKBACK_Damage = 100;
	const int32 INVENCAP = 5;
	const TArray<int32> WEAPONDAMAGE = { 0, 150, 200, 160, 100 };
	const TArray<int32> WEAPONRADIUS = { 0, 80, 95, 65, 50 };
	const TArray<int32> WEAPONRANGE = { 0, 130, 145, 115, 100 };

	int32 PotionCount = 5;
	bool canNextCombo;
	bool isInCombo;
	bool InventoryDelay = false;
	bool CheckInven = false;
	bool IsOpen = false;
	float Fov = 90.0f;

	class AEnemyBase* lockedTarget;
	class AEnemyBase* TimeStopEnemy;
	class UAIPerceptionStimuliSourceComponent* stimulusSource;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainWidget> GameHUD; 

	void MoveVertical(float value);
	void MoveHorizontal(float value);
	void LookUp(float value);
	void Turn(float value);
	void Attack();
	void AttackCheck();
	void EndCombo();
	void Run();
	void Dodge();
	void ReturnState();
	void LockOnTarget();
	void LookTargetCamera(float deltaSeconds) const;
	void LookTarget(float deltaSeconds) const;
	void StartRun();
	void EndRun();
	void PlayMontage(FName sectionName);
	void SpaceAction();
	void BackStab();
	void PickupItems();
	void EnableMouseCursor();
	void DisableMouseCursor();
	void TimeStop();
	void KnockBackSkill();
	void VisibleWeapon();
	void UnVisibleWeapon();
	void KnockBackOn();
	void DrinkPotion();
	void UseAssertSkill();
	void SpawnGhostTail();

	UFUNCTION()
	void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
