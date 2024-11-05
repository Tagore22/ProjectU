#include "PlayerAnim.h"

UPlayerAnim::UPlayerAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> findPlayerMontage(TEXT("AnimMontage'/Game/Character/Player/Animation/PlayerMontage.PlayerMontage'"));
	if (findPlayerMontage.Succeeded())
		playerMontage = findPlayerMontage.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> findPotionMontage(TEXT("AnimMontage'/Game/Character/Player/Animation/Player_PotionMontage.Player_PotionMontage'"));
	if (findPotionMontage.Succeeded())
		PotionMontage = findPotionMontage.Object;
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto onwerPawn = TryGetPawnOwner();
	auto player = Cast<APUPlayer>(onwerPawn);
	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector f_Vec = player->f_Vec;
		FVector r_Vec = player->r_Vec;
		if (isLockOn && mState != EPlayerState::Run)
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

void UPlayerAnim::AnimNotify_AttackHitCheck()
{
	attackHitCheck.Broadcast();
}

void UPlayerAnim::AnimNotify_DodgeEnd()
{
	dodgeEnd.Broadcast();
}

void UPlayerAnim::AnimNotify_DamagedEnd()
{
	damagedEnd.Broadcast();
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
	attackEnd.Broadcast();
}

void UPlayerAnim::AnimNotify_BackStabEnd()
{
	backStabEnd.Broadcast();
}

void UPlayerAnim::AttackStop()
{
	Montage_Stop(0.25f, playerMontage);
}

void UPlayerAnim::AnimNotify_KnockBackStart()
{
	knockBackStart.Broadcast();
}

void UPlayerAnim::AnimNotify_KnockBackSkillEnd()
{
	knockBackSkillEnd.Broadcast();
}

void UPlayerAnim::AnimNotify_potionEnd()
{
	potionEnd.Broadcast();
}

void UPlayerAnim::AnimNotify_healEnd()
{
	healEnd.Broadcast();
}

void UPlayerAnim::AnimNotify_assertCheck()
{
	assertCheck.Broadcast();
}

void UPlayerAnim::AnimNotify_assertEnd()
{
	assertEnd.Broadcast();
}

void UPlayerAnim::AnimNotify_spawnGhostTail()
{
	spawnGhostTail.Broadcast();
}