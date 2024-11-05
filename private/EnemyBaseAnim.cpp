#include "EnemyBaseAnim.h"
#include "EnemyBase.h"
#include "EnemyBaseAIController.h"
#include "PUPlayer.h"
#include "Kismet/GameplayStatics.h"

void UEnemyBaseAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (me)
	{
		FVector velocity = me->GetVelocity();
		isTarget = me->isTarget;
		FVector f_Vec = me->f_Vec;
		FVector r_Vec = me->r_Vec;
		if (isTarget)
		{
			speed = FVector::DotProduct(f_Vec, velocity);
			direction = FVector::DotProduct(r_Vec, velocity);
		}
		else
		{
			speed = velocity.Size();
		}
	}
}

void UEnemyBaseAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	me = Cast<AEnemyBase>(TryGetPawnOwner());
}

void UEnemyBaseAnim::AnimNotify_EnemyDie()
{
	me->isDieEnd = true;
}

void UEnemyBaseAnim::AnimNotify_DamagedEnd()
{
	auto con = Cast<AEnemyBaseAIController>(me->GetController());
	me->isTarget = true;
	con->RunAI();
}

void UEnemyBaseAnim::AnimNotify_AttackHit()
{
	attackHit.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_AttackStart()
{
	attackStart.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_AttackEnd()
{
	attackEnd.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_KnockBackEnd()
{
	auto con = Cast<AEnemyBaseAIController>(me->GetController());
	me->isTarget = true;
	if (con != nullptr)
		con->RunAI();
}

void UEnemyBaseAnim::AnimNotify_TelpoEnd()
{
	telpoEnd.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_MeteorStart()
{
	meteorStart.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_MeteorEnd()
{
	meteorEnd.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_SummonStart()
{
	summonStart.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_SummonEnd()
{
	summonEnd.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_BoltStart()
{
	boltStart.Broadcast();
}

void UEnemyBaseAnim::AnimNotify_BoltEnd()
{
	boltEnd.Broadcast();
}