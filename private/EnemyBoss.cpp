#include "EnemyBoss.h"
#include "EnemyBossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyBaseAnim.h"
#include "Bolt.h"
#include "Meteor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PUPlayer.h"
#include "EnemySword.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "EnemyHPBar.h" 
#include "Components/TextBlock.h"

AEnemyBoss::AEnemyBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> findMesh(TEXT("SkeletalMesh'/Game/Character/Enemy/EnemyBoss/Mesh/EnemyBoss.EnemyBoss'"));
	if (findMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(findMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> findAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_EnemyBoss.ABP_EnemyBoss_C'"));
	if (findAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(findAnim.Class);

	AIControllerClass = AEnemyBossAIController::StaticClass();
	static ConstructorHelpers::FObjectFinder<UAnimMontage> findMontage(TEXT("AnimMontage'/Game/Character/Enemy/EnemyBoss/Animation/EnemyBoss_Montage.EnemyBoss_Montage'"));
	if (findMontage.Succeeded())
		montage = findMontage.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FindTelpo(TEXT("NiagaraSystem'/Game/VFX/N_BossTelpo.N_BossTelpo'"));
	if (FindTelpo.Succeeded())
	{
		TelpoSystem = FindTelpo.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FindSummon(TEXT("NiagaraSystem'/Game/VFX/N_BossSummon.N_BossSummon'"));
	if (FindSummon.Succeeded())
	{
		SummonSystem = FindSummon.Object;
	}
	damage = BOSS_DAMAGE;
	isBoss = true;
	hp *= BOSS_HP_MULTI;
	InitHP *= BOSS_HP_MULTI;
}

void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	f_Vec = GetActorForwardVector();
	r_Vec = GetActorRightVector();
}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	anim->telpoEnd.AddLambda([this]()->void
		{
			if (hp <= 0)
				return;
			float X = FMath::RandRange(-44500, -48000);
			float Y = FMath::RandRange(14000, 17000);
			SetActorLocation(FVector(X, Y, GetActorLocation().Z));
			mState = EEnemyState::Idle;
			isTarget = true;
			IsTimerON = false;
			auto con = Cast<AEnemyBossAIController>(GetController());
			con->RunAI();
		});
	anim->meteorStart.AddLambda([this]()->void
		{
			isTarget = false;
			SpawnMeteor();
		});
	anim->meteorEnd.AddLambda([this]()->void
		{
			isTarget = true;
		});
	anim->summonStart.AddLambda([this]()->void
		{
			isTarget = false;
			SpawnSummon();
		});
	anim->summonEnd.AddLambda([this]()->void
		{
			isTarget = true;
		});
	anim->boltStart.AddLambda([this]()->void
		{
			isTarget = false;
			SpawnBolt();
			
		});
	anim->boltEnd.AddLambda([this]()->void
		{
			isTarget = true;
		});
}

void AEnemyBoss::OnDamageProcess(int32 Damage, bool IsKnockBack)
{
	FTimerHandle TelpoTimer;
	// 타이머를 넣는다. 다만 이전 타이머를 지우지 않고 단 한번만 사용되어야 한다.
	if (!IsTimerON)
	{
		IsTimerON = true;
		GetWorld()->GetTimerManager().SetTimer(TelpoTimer, FTimerDelegate::CreateLambda([&]()->void {
			CastTelPo();
			}), 3.0f, false);
	}
	isBoss = hp - Damage > 0 ? true : false;
	Super::OnDamageProcess(Damage, false);
}

void AEnemyBoss::CastTelPo()
{
	if (hp <= 0)
		return;
	mState = EEnemyState::SuperArmor;
	// 여기서 텔포 이펙트(파티클)을 띄운다.
	TelpoComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TelpoSystem, GetActorLocation());
	PlayMontage(TEXT("Telpo"));
	FTimerHandle TelpoEndTimer;
	GetWorld()->GetTimerManager().SetTimer(TelpoEndTimer, FTimerDelegate::CreateLambda([&]()->void {
		TelpoComponent->Deactivate();
		}), 2.2f, false);
	// 그 후 텔포 애니메이션을 실행한다. 애니메이션이 끝나면 텔포 파티클을 끈다.
}

void AEnemyBoss::CastSummon()
{
	// 소환 이펙트를 스폰한다.
	// 소환 애니메이션을 재생한다.
	PlayMontage(TEXT("Summon"));
	// 몹들을 소환할 위치를 구한다. 대략 4기정도.
	// 각각 for문을 돌며 스폰하고 이펙트 또한 같이 스폰한다.
}

void AEnemyBoss::CastMeteor()
{
	// 메테오 이펙트를 스폰한다.
	// 메테오 애니메이션을 재생한다.
	PlayMontage(TEXT("Meteor"));
	// 메테오를 스폰한다. 이것은 EnemyBow의 Arrow와 비슷하지만 스폰 위치가 꽤나 위에서 소환되어야 한다.
}

void AEnemyBoss::CastBolt()
{
	// 메테오 이펙트를 스폰한다.
	// 메테오 애니메이션을 재생한다.
	PlayMontage(TEXT("Bolt"));
	// 메테오를 스폰한다. 이것은 EnemyBow의 Arrow와 비슷하지만 스폰 위치가 꽤나 위에서 소환되어야 한다.
}

void AEnemyBoss::SpawnBolt()
{
	FTransform boltPosition = GetMesh()->GetSocketTransform(TEXT("BoltSocket"));
	GetWorld()->SpawnActor<ABolt>(ABolt::StaticClass(), boltPosition);
}

void AEnemyBoss::SpawnMeteor()
{
	auto MeteorTarget = Cast<APUPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	GetWorld()->SpawnActor<AMeteor>(AMeteor::StaticClass(), MeteorTarget->GetActorTransform());
}

void AEnemyBoss::SpawnSummon()
{
	float cur = SPAWN_LOC;
	for (int i = 0; i < 4; ++i)
	{
		FVector loc = GetActorLocation() + GetActorRightVector() * cur + GetActorForwardVector() * SPAWN_LOC;
		SummonComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SummonSystem, FVector(loc.X, loc.Y, loc.Z + 30), FRotator::ZeroRotator, FVector(3.5, 3.5, 3.5));
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		auto spawned = GetWorld()->SpawnActor<AEnemySword>(AEnemySword::StaticClass(), loc, GetActorRotation(), ActorSpawnParams);
		if (spawned != nullptr)
		{
			spawned->Spawned();
		}
		cur -= SPAWN_BETWEEN;
	}
}

void AEnemyBoss::InitBossHPBar()
{
	HPRef = CreateWidget<UEnemyHPBar>(GetWorld(), HPHUD);
	HPRef->AddToViewport();
	HPRef->HPRatio = 1;
	HPRef->DamageText->SetVisibility(ESlateVisibility::Hidden);
}