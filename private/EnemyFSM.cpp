#include "EnemyFSM.h"
/*#include "Kismet/GameplayStatics.h"
#include "PUPlayer.h"
#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "EnemyBaseAnim.h"
#include "AIController.h"
#include "NavigationSystem.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto player = UGameplayStatics::GetActorOfClass(GetWorld(), APUPlayer::StaticClass());
	target = Cast<APUPlayer>(player);
	me = Cast<AEnemyBase>(GetOwner());
	anim = Cast<UEnemyBaseAnim>(me->GetMesh()->GetAnimInstance());
	anim->attackHit.AddLambda([this]()->void
		{
			target->OnHitEvent();
		});
	ai = Cast<AAIController>(me->GetController());
}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > idleDelayTime)
	{
		mState = EEnemyState::Move;
		currentTime = 0;
		anim->animState = mState;
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemyFSM::MoveState()
{
	FVector to = target->GetActorLocation();
	FVector dir = to - me->GetActorLocation();

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FAIMoveRequest req;
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(to);

	FPathFindingQuery query;
	ai->BuildPathfindingQuery(req, query);

	FPathFindingResult r = ns->FindPathSync(query);

	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(to);
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos);
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}

	if (dir.Size() <= attackRange)
	{
		ai->StopMovement();
		mState = EEnemyState::Attack;
		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime)
	{
		UE_LOG(LogTemp, Log, TEXT("EnemyAttack!"));
		currentTime = 0;
		anim->bAttackPlay = true;
	}

	float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (dist > attackRange)
	{
		mState = EEnemyState::Move;
		anim->animState = mState;
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
	if (!anim->isDie)
		return;
	FVector moveVec = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector cur = me->GetActorLocation();
	FVector next = moveVec + cur;
	me->SetActorLocation(next);

	if (next.Z <= -200.0f)
		me->Destroy();
}

void UEnemyFSM::OnDamageProcess()
{
	--hp;
	if (hp > 0)
	{
		currentTime = 0;
		mState = EEnemyState::Damage;
		int32 idx = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damaged%d"), idx);
		//me->PlayEnemyBaseMontage(FName(*sectionName));
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//me->PlayEnemyBaseMontage(TEXT("Die"));
	}
	anim->animState = mState;
	ai->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& Dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(CenterLocation, Radius, loc);
	Dest = loc.Location;
	return result;
}*/
