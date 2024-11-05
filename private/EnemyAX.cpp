#include "EnemyAX.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAXAIController.h"
#include "PUPlayer.h"
#include "EnemyBaseAnim.h"

AEnemyAX::AEnemyAX()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> findMesh(TEXT("SkeletalMesh'/Game/Character/Enemy/EnemyBase/Mesh/EnemyBase.EnemyBase'"));
	if (findMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(findMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.7, 0.7, 0.7));
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> findAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_EnemyBase.ABP_EnemyBase_C'"));
	if (findAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(findAnim.Class);
	AIControllerClass = AEnemyAXAIController::StaticClass();
	static ConstructorHelpers::FObjectFinder<UAnimMontage> findMontage(TEXT("AnimMontage'/Game/Character/Enemy/EnemyBase/Animation/EnemyBase_Montage.EnemyBase_Montage'"));
	if (findMontage.Succeeded())
		montage = findMontage.Object;

	walkSpeed = AX_WALKSPEED;
	runSpeed = AX_RUNSPEED;
	attackRange = AX_ATTACKRANGE;
	attackRadius = AX_ATTACKRADIUS;
	minCombo = AX_MINCOMBO;
	maxCombo = AX_MAXCOMBO;
	targetSpeed = AX_TARGETSPEED;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	damage = AX_DAMAGE;
}

void AEnemyAX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	f_Vec = GetActorForwardVector();
	r_Vec = GetActorRightVector();
}

void AEnemyAX::BeginPlay()
{
	Super::BeginPlay();

	if (anim != nullptr)
		anim->attackHit.AddUObject(this, &AEnemyAX::EnemyAttack);
}