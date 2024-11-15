# Unreal Portfolio

* 엔진 : Unreal Engine 4.27
* 에디터 : Microsoft Visual Studio Community 2019 16.11.23
* 제작기간 : 2024. 6. 27 ~ 2024. 11. 3
* 1인 자체개발

# 데모 영상
> Youtube


[![ReadMe 이미지(TemplateItem_2)](https://github.com/user-attachments/assets/e9a6ddf3-7c62-455d-84ef-4aede15deb30)](https://www.naver.com/)
# 기술설명서
## Controller - Animation - Character 구조
![구조 관계도](https://github.com/user-attachments/assets/a6e1e95c-812f-42a7-b25d-8e4db5d3a6b0)
## EnemyBase
* HPBarWidget 관리
* Enemy의 FSM들을 설정
* Attack, Run, Damaged, Die, Targeting등의 액션이 구현되어 있으며 상속된 자식클래스에서 활용함
* UEnemyBaseAnim을 사용하여 파생클래스들의 Notify Delegate Broadcast
* AIPerception과 Behavior tree를 이용하여 Player를 감지 및 공격하거나 일정 위치를 순찰함
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
> Animation BP
* Blend Space 활용

![Enemy ABP](https://github.com/user-attachments/assets/34543596-bf8e-4008-9d44-05c362318c0d)
> Enemy AI Behavior Tree

![Enemy 비헤이비어 트리](https://github.com/user-attachments/assets/15998a89-1e3e-4921-8038-7d885a931b77)
> EnemyMontage
* Animation Notify 사용
  
![EnemyMontage](https://github.com/user-attachments/assets/28a56b5f-9c67-40b1-8de2-c6d64576235c)
## PUPlayer
* HPBarWidget 관리
* Attack, Run, Damaged, Die, LockOn, Potion, Assert, Dodge, BackStab, KnockBack, GhostTail 액션
* Inventory를 이용하여 무기를 습득 및 교체
> LockOnTarget
```c++
void APUPlayer::LockOnTarget()
{
	if(lockedTarget != nullptr)
	{
		lockedTarget->TimerHPBar();
		lockedTarget->LockedOn(false);
		lockedTarget->isTargeted = false;
		anim->isLockOn = false;
		lockedTarget = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		FVector curVec = camera->GetForwardVector().RotateAngleAxis(-60.f, FVector::UpVector);
		FVector cameraLoc = camera->GetComponentLocation();
		FVector startLoc = FVector(cameraLoc.X, cameraLoc.Y, cameraLoc.Z - 90);
		TArray<TEnumAsByte<EObjectTypeQuery>> lockedObject;
		lockedObject.Add(EObjectTypeQuery::ObjectTypeQuery3);
		lockedObject.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		TArray<AActor*> ignoredActors;
		ignoredActors.Add(this);
		FHitResult hitResult;
		float minDist = 1000.0f;
		AEnemyBase* findLock = nullptr;
		for (int i = 0; i < 120; i += 5) // 일정 범위에 트레이스를 쏴서 가장 가까운 적을 찾는다.
		{
			FVector dir = curVec.RotateAngleAxis(i, FVector::UpVector);
			FVector endLoc = startLoc + dir * LOCKONRADIUS;
			bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
				GetWorld(), startLoc, endLoc, LOCKONDIST, lockedObject, false, ignoredActors,
				EDrawDebugTrace::ForDuration, hitResult, true, FLinearColor::Red, FLinearColor::Green, 2.0f);
			auto next = Cast<AEnemyBase>(hitResult.GetActor());//
			if (bHit && hitResult.Distance < minDist && next != nullptr && next->isAlive)
			{
				minDist = hitResult.Distance;
				findLock = next;
			}
		}
		if (findLock != nullptr)
		{
			anim->isLockOn = true;
			lockedTarget = findLock;
			lockedTarget->VisibleHPBar();
			lockedTarget->LockedOn(true);
			lockedTarget->isTargeted = true; 
			if(mState != EPlayerState::Run)
				GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
}
```
> ToggleInventory
```c++
void APUPlayer::ToggleInventory()
{
	if (HUDRef->ActivateInventory && !InventoryDelay)
	{
		FTimerHandle DelayTimer;
		if (!CheckInven)
		{
			HUDRef->InventoryVisible = ESlateVisibility::Visible;
			EnableMouseCursor();
			HUDRef->PlayAnimation(HUDRef->MenuAnim);
			InventoryDelay = true;
			IsOpen = true;
			CheckInven = true;

			GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()->void {
				InventoryDelay = false;
				if (IsOpen)
				{
					UGameplayStatics::SetGamePaused(GetWorld(), true);
				}
				GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
				}), 0.5f, false);
		}
		else
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			IsOpen = false;
			InventoryDelay = true;
			HUDRef->PlayAnimation(HUDRef->MenuAnim, 0, 1, EUMGSequencePlayMode::Reverse, 1, false);
			DisableMouseCursor();
			CheckInven = false;
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()->void {
				HUDRef->InventoryVisible = ESlateVisibility::Hidden;
				GetWorld()->GetTimerManager().ClearTimer(DelayTimer);

				GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()->void {
					InventoryDelay = false;
					if (IsOpen)
					{
						UGameplayStatics::SetGamePaused(GetWorld(), true);
					}
					GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
					}), 0.5f, false);
				}), 1.0f, false);
		}
	}
}
```
> Animation BP
* Blend Space, Bone Layer Blend 활용
  
![플레이어 ABP](https://github.com/user-attachments/assets/272f5b3d-e893-4e1a-8f53-4d77846e183d)
![플레이어 Animation](https://github.com/user-attachments/assets/183e1f59-7659-475f-adb8-2b87fc145d9f)
> PlayerMontage

* Animation Notify 사용

![PlayerMontage](https://github.com/user-attachments/assets/72466d89-d27a-4959-8112-3d7ad0254973)
## EnemyBaseAIController
* Enemy가 감지하는 시야 및 청각 관리
* Behavior tree 및 블랙보드 관리
* Behavior tree에서 사용되는 UFUNCTION(BlueprintCallable)계열 함수들 관리
> AEnemyBaseAIController
```c++
AEnemyBaseAIController::AEnemyBaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config")); // 시야 관리.
	sightConfig->SightRadius = AI_SIGHT_RADIUS; 
	sightConfig->LoseSightRadius = AI_LOSE_SIGHT_RADIUS;
	sightConfig->PeripheralVisionAngleDegrees = AI_FIELD_OF_VIEW;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->SetMaxAge(AI_MAXAGE);

	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config")); // 청각 관리.
	hearingConfig->HearingRange = AI_HEARING_RANGE;
	hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	hearingConfig->SetMaxAge(AI_MAXAGE);

	GetPerceptionComponent()->ConfigureSense(*hearingConfig);
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
	GetPerceptionComponent()->SetDominantSense(sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyBaseAIController::OnTargetDetected);
}
```
> CalAttackDelay
```c++
void AEnemyBaseAIController::CalAttackDelay(float deltaTime)
{
	curTime += deltaTime;
	if (curTime >= attackDelayTime)
	{
		isAttackDelay = false;
		GetBlackboardComponent()->SetValueAsBool("IsAttackDelay", false);
		attackDelayTime = FMath::RandRange(MINATTACKDELAY, MAXATTACKDELAY);
		curTime = 0.0f;
	}
}
```
> OnTargetDetected
```c++
void AEnemyBaseAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (!isReady)
		return;
	auto player = Cast<APUPlayer>(Actor);
	if (player)
	{
		auto me = Cast<AEnemyBase>(GetPawn());
		me->target = player;
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetPawn"), Actor);
	}
}
```
## TemplateItem
* 장비의 습득, 사용, 버릴때의 관리
* 인터페이스 클래스 사용
> PickupEvent
```c++
void ATemplateItem::PickupEvent()
{
	if (GetActorEnableCollision() && Isinrange)
	{
		PlayerRef->HUDRef->Inventory.Add(Iteminfo);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}
```
> OnEndOverlap
```c++
void ATemplateItem::OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto player = Cast<APUPlayer>(OtherActor);
	if (player != nullptr)
	{
		Widget->SetVisibility(false);
		Isinrange = false;
		Mesh->SetRenderCustomDepth(false);
	}
}
```
> Blueprint

![ReadMe 이미지(TemplateItem_1)](https://github.com/user-attachments/assets/c058e26d-5cb4-48f7-a849-b758b761e832)
![ReadMe 이미지(TemplateItem_2)](https://github.com/user-attachments/assets/e9a6ddf3-7c62-455d-84ef-4aede15deb30)
## MainWidget
* Player의 HP, MP, Stamina와 남은 포션의 개수 및 인벤토리를 관리
* Player의 정보를 연동하여 실시간으로 반영
> Blueprint

![MainWidget BP](https://github.com/user-attachments/assets/6cae93a1-79db-4bc9-9c33-3c0d601c19cc)
## DestructibleProp
* Overlap시 부서지는 아이템들의 관리
* UDestructibleComponent 사용
* OnOverlapBegin의 오버라이드 사용
> ADestructibleProp
```c++
ADestructibleProp::ADestructibleProp()
{
	Coll = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Coll->SetCollisionProfileName("Barrier");
	RootComponent = Coll;

	DM = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Mesh"));
	DM->SetupAttachment(RootComponent);
	DM->SetNotifyRigidBodyCollision(true);
}
```
> OnOverlapBegin
```c++
void ADestructibleProp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FTimerHandle DMTimer;
	GetWorld()->GetTimerManager().SetTimer(DMTimer, FTimerDelegate::CreateLambda([&]()->void {
		Destroy();
		}), 2.5f, false);
}
```
