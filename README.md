# Unreal Portfolio

* 엔진 : Unreal Engine 4.27
* 에디터 : Microsoft Visual Studio Community 2019 16.11.23
* 제작기간 : 2024. 6. 27 ~ 2024. 11. 3
* 1인 자체개발

# 데모 영상
> Youtube
# 기술설명서
## Controller - Animation - Character 구조
![구조 관계도](https://github.com/user-attachments/assets/a6e1e95c-812f-42a7-b25d-8e4db5d3a6b0)
## EnemyBase
* HPBarWidget 관리
* Enemy의 FSM들을 설정
* Attack, Run, Damaged, Die, Targeting등의 액션이 구현되어 있으며 상속된 자식클래스에서 활용함
* UEnemyBaseAnim을 사용하여 파생클래스들의 Notify Delegate Broadcast
> OnDamageProcess
```c++
void AEnemyBase::OnDamageProcess(int32 Damage, bool IsKnockBack)
{
	hp -= Damage;
	UpdateHP(Damage);
	auto con = Cast<AEnemyBaseAIController>(GetController());
	con->StopAI();
	isTarget = false;
	if (hp <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayMontage(TEXT("Die"));
		isAlive = false;
	}
	else
	{
		if (mState == EEnemyState::SuperArmor) // 텔포시 슈퍼아머가 되는데 이때 피격 애니메이션은 무시된다.
		{
			return;
		}
		if (IsKnockBack)
		{
			PlayMontage(TEXT("KnockBack"));
		}
		else
		{
			int32 idx = FMath::RandRange(MINDAMAGE, MAXDAMAGE);
			FString sectionName = FString::Printf(TEXT("Damaged%d"), idx);
			PlayMontage(FName(*sectionName));
		}
	}
}
```
> EnemyAttack
```c++
void AEnemyBase::EnemyAttack()
{
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(
		hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * attackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(attackRadius),
		params);

	// 공격 범위용 트레이스.
	/*FVector traceVec = GetActorForwardVector() * attackRange;
	FVector center = GetActorLocation() + traceVec * 0.5;
	float halfHeight = attackRange * 0.5 + attackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bHit ? FColor::Green : FColor::Red;
	float debugLifeTime = 5;

	DrawDebugCapsule(GetWorld(),
		center,
		halfHeight,
		attackRadius,
		capsuleRot,
		drawColor,
		false,
		debugLifeTime);*/

	if (bHit)
	{
		isTarget = false;
		auto attackTarget = hitResult.GetActor();
		if (attackTarget->GetName().Contains("Player"))
		{
			auto player = Cast<APUPlayer>(attackTarget);
			player->OnHitEvent(damage);
		}
	}
}
```
