#include "EnemyBow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyBowAIController.h"
#include "Arrow.h"
#include "EnemyBaseAnim.h"
#include "PUPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyBow::AEnemyBow()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> findMesh(TEXT("SkeletalMesh'/Game/Character/Enemy/EnemyBow/Mesh/EnemyBow.EnemyBow'"));
	if (findMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(findMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.7, 0.7, 0.7));
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> findAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_EnemyBow.ABP_EnemyBow_C'"));
	if (findAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(findAnim.Class);
	AIControllerClass = AEnemyBowAIController::StaticClass();
	static ConstructorHelpers::FObjectFinder<UAnimMontage> findMontage(TEXT("AnimMontage'/Game/Character/Enemy/EnemyBow/Animation/EnemyBow_Montage.EnemyBow_Montage'"));
	if (findMontage.Succeeded())
		montage = findMontage.Object;
	walkSpeed = BOW_WALKSPEED;
	runSpeed = BOW_RUNSPEED;
	minCombo = BOW_MINCOMBO;
	maxCombo = BOW_MAXCOMBO;
	targetSpeed = BOW_TARGETSPEED;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	damage = BOW_DAMAGE;
}

void AEnemyBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	f_Vec = GetActorForwardVector();
	r_Vec = GetActorRightVector();
	if (isTarget)
		ShootTrace();
	if (target != nullptr && !canSee)
	{
		sightDelay += DeltaTime;
	}
	else
	{
		sightDelay = 0.0f;
	}
}

void AEnemyBow::BeginPlay()
{
	Super::BeginPlay();

	anim->attackHit.AddLambda([this]()->void
		{
			SpawnArrow();
			isTarget = false;
		});
}

void AEnemyBow::EnemyAttack()
{
	PlayAttackMontage();
}

void AEnemyBow::SpawnArrow() const
{
	FTransform arrowPosition = GetMesh()->GetSocketTransform(TEXT("ArrowSocket"));
	GetWorld()->SpawnActor<AArrow>(arrow, arrowPosition);
}

void AEnemyBow::ShootTrace() 
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + GetActorForwardVector() * BOW_SIGHTDISTANCE;
	FHitResult hitInfo;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	bool bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), startPos, endPos, FVector(70.0f, 70.0f, 70.0f), GetActorRotation(),
		ETraceTypeQuery::TraceTypeQuery1, false, actorsToIgnore, EDrawDebugTrace::None, hitInfo, true, FLinearColor::Red, FLinearColor::Green);
	if (bHit)
	{
		auto player = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("SpringArm"));
		canSee = player != nullptr ? true : false;
	}
	else
	{
		canSee = false;
	}
}

void AEnemyBow::ResetSight()
{
	sightDelay = 0;
	canSee = false;
}