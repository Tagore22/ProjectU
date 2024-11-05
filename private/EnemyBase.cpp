#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "NavigationInvokerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PUPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBaseAnim.h"
#include "DrawDebugHelpers.h"
#include "EnemyBaseAIController.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "EnemyHPBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	rot = GetActorRotation();
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(RootComponent);
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UEnemyHPBar> FindHPbar(TEXT("WidgetBlueprint'/Game/Blueprints/BP_EnemyHP.BP_EnemyHP_C'"));
	if (FindHPbar.Succeeded())
	{
		HPBar->SetWidgetClass(FindHPbar.Class);
		HPBar->SetDrawSize(FVector2D(100.0f, 10.0f));
	}
	LockWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockWidget"));
	LockWidget->SetupAttachment(RootComponent);
	LockWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> FindLock(TEXT("WidgetBlueprint'/Game/Blueprints/WB_LockWidget.WB_LockWidget_C'"));
	if (FindLock.Succeeded())
	{
		LockWidget->SetWidgetClass(FindLock.Class);
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (hp <= 0)
		Die();
	if (isTarget)
	{
		LookTarget();
	}
	else
	{
		rot = GetActorRotation();
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	homeLocation = GetActorLocation();
	anim = Cast<UEnemyBaseAnim>(GetMesh()->GetAnimInstance());
	if (anim != nullptr)
	{
		anim->attackStart.AddUObject(this, &AEnemyBase::LookTarget);
		anim->attackEnd.AddLambda([this]()->void
			{
				isTarget = true;
			});
	}
}

void AEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HPBar->InitWidget();
	// 여기 이 부분에서 옮길것 있나 나중에 확인해볼것.
	HPRef = Cast<UEnemyHPBar>(HPBar->GetUserWidgetObject());
	HPRef->SetVisibility(ESlateVisibility::Hidden);
	HPRef->Damaged = 0;
	HPRef->DamageText->SetText(FText::AsNumber(HPRef->Damaged));
	HPRef->HPRatio = 1;
	LockWidget->InitWidget();
	LockRef = Cast<UUserWidget>(LockWidget->GetUserWidgetObject());
	LockRef->SetVisibility(ESlateVisibility::Hidden);
}

// 일단은 공격용 몽타주만 구현하였음. 후에 다른 몽타주가 필요하게 되어 통합할수도 있음.
// 내가 정한 섹션의 수는 1 ~ 3이지만 실제로는 0 ~ 2가 된다. 따라서 현재 섹션의 길이를 시간으로 구하고자 한다면
// 다음 섹션의 시작시간 - 현재 섹션의 시작시간을 구하면 된다.
float AEnemyBase::PlayAttackMontage()
{
	int32 idx = FMath::RandRange(minCombo, maxCombo);
	FString sectionName = FString::Printf(TEXT("Attack%d"), idx);
	float curTime = montage->GetAnimCompositeSection(idx - 1).GetTime();
	float nextTime = montage->GetAnimCompositeSection(idx).GetTime();
	PlayMontage(FName(*sectionName));
	return nextTime - curTime;
}

void AEnemyBase::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	mState = EEnemyState::Run;
	anim->mState = mState;
}

void AEnemyBase::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	mState = EEnemyState::Idle;
	anim->mState = mState;
}

void AEnemyBase::BeginTarget()
{
	isTarget = true;
	GetCharacterMovement()->MaxWalkSpeed = targetSpeed;
}

void AEnemyBase::EndTarget()
{
	isTarget = false;
}

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

void AEnemyBase::PlayMontage(FName sectionName)
{
	PlayAnimMontage(montage, 1.0, sectionName);
}

void AEnemyBase::Die()
{
	if (!isDieEnd)
		return;
	FVector moveVec = FVector::DownVector * DIESPEED * GetWorld()->DeltaTimeSeconds;
	FVector cur = GetActorLocation();
	FVector next = moveVec + cur;
	SetActorLocation(next);

	if (next.Z <= -200.0f)
		Destroy();
}

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

void AEnemyBase::LookTarget()
{
	FVector lockedOnLocation = target->GetActorLocation();
	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), lockedOnLocation);
	FRotator interpRotation = UKismetMathLibrary::RInterpTo(rot, lookAtRotation, GetWorld()->DeltaTimeSeconds, ROTSPEED);
	GetController()->GetPawn()->SetActorRotation(FRotator(rot.Pitch, interpRotation.Yaw, rot.Roll));
	rot = FRotator(rot.Pitch, interpRotation.Yaw, rot.Roll);
}

void AEnemyBase::UpdateHP(int32 Damage)
{
	float CurHP = hp;
	HPRef->HPRatio = hp / InitHP;
	HPRef->Damaged += Damage;
	HPRef->DamageText->SetText(FText::AsNumber(HPRef->Damaged));
	HPRef->DamageText->SetVisibility(ESlateVisibility::Visible);
	TimerDamageFloat();
	if (!isTargeted)
	{
		TimerHPBar();
	}
}

void AEnemyBase::VisibleHPBar()
{
	if (isBoss)
		return;
	GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
	HPRef->SetVisibility(ESlateVisibility::Visible);
	TimerDamageFloat();
	if (HPRef->Damaged == 0)
	{
		HPRef->DamageText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AEnemyBase::TimerHPBar()
{
	if (isBoss)
		return;
	VisibleHPBar();

	GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()->void {
		HPRef->SetVisibility(ESlateVisibility::Hidden);
		HPRef->Damaged = 0;
		HPRef->DamageText->SetVisibility(ESlateVisibility::Hidden);
		}), 2.0f, false);
}

void AEnemyBase::TimerDamageFloat()
{
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()->void {
		HPRef->DamageText->SetVisibility(ESlateVisibility::Hidden);
		HPRef->Damaged = 0;
		}), 2.0f, false);
}

void AEnemyBase::LockedOn(bool IsLocked)
{
	if (IsLocked)
	{
		LockRef->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LockRef->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AEnemyBase::Spawned() 
{
	auto con = Cast<AEnemyBaseAIController>(GetController());
	con->Spawned();
}