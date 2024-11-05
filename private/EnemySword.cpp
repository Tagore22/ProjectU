#include "EnemySword.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemySwordAIController.h" 
#include "PUPlayer.h"
#include "EnemyBaseAnim.h"
#include "Components/CapsuleComponent.h"

AEnemySword::AEnemySword()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> findMesh(TEXT("SkeletalMesh'/Game/Character/Enemy/EnemySword/Mesh/EnemySword.EnemySword'"));
	if (findMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(findMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> findAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_EnemySword.ABP_EnemySword_C'"));
	if (findAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(findAnim.Class);
	AIControllerClass = AEnemySwordAIController::StaticClass();
	static ConstructorHelpers::FObjectFinder<UAnimMontage> findMontage(TEXT("AnimMontage'/Game/Character/Enemy/EnemySword/Animation/EnemySword_Montage.EnemySword_Montage'"));
	if (findMontage.Succeeded())
		montage = findMontage.Object;
	GetCapsuleComponent()->SetCapsuleRadius(55);
	walkSpeed = SWORD_WALKSPEED;
	runSpeed = SWORD_RUNSPEED;
	attackRange = SWORD_ATTACKRANGE;
	attackRadius = SWORD_ATTACKRADIUS;
	minCombo = SWORD_MINCOMBO;
	maxCombo = SWORD_MAXCOMBO;
	targetSpeed = SWORD_TARGETSPEED;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	damage = SWORD_DAMAGE;
}

void AEnemySword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	f_Vec = GetActorForwardVector();
	r_Vec = GetActorRightVector();
}

void AEnemySword::BeginPlay()
{
	Super::BeginPlay();

	if (anim != nullptr)
		anim->attackHit.AddUObject(this, &AEnemySword::EnemyAttack);
}