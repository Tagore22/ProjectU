#include "Meteor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PUPlayer.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
	coll = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = coll;
	coll->SetSphereRadius(200.0f);
	coll->SetCollisionProfileName(TEXT("Arrow"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FindVFX(TEXT("NiagaraSystem'/Game/VFX/N_BossMeteor1.N_BossMeteor1'"));
	if (FindVFX.Succeeded())
	{
		VFXSystem = FindVFX.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FindMagmaVFX(TEXT("NiagaraSystem'/Game/VFX/N_BossMeteorMagma.N_BossMeteorMagma'"));
	if (FindMagmaVFX.Succeeded())
	{
		MagmaVFXSystem = FindMagmaVFX.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FindEarthVFX(TEXT("NiagaraSystem'/Game/VFX/N_BossMeteorEarth.N_BossMeteorEarth'"));
	if (FindEarthVFX.Succeeded())
	{
		EarthVFXSystem = FindEarthVFX.Object;
	}
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> FindCameraShake(TEXT("Blueprint'/Game/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	if (FindCameraShake.Succeeded())
	{
		CameraShake = FindCameraShake.Class;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> findMeteorSound(TEXT("SoundWave'/Game/Sounds/ExplosionSound.ExplosionSound'"));
	if (findMeteorSound.Succeeded())
		MeteorSound = findMeteorSound.Object;
}

void AMeteor::BeginPlay()
{
	Super::BeginPlay();

	coll->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::OnOverlapBegin);
	VFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXSystem, GetActorLocation());
	FTimerHandle MeteorTimer;
	GetWorld()->GetTimerManager().SetTimer(MeteorTimer, FTimerDelegate::CreateLambda([&]()->void {
		VFXComponent->Deactivate();
		MagmaVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MagmaVFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 400));
		EarthVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EarthVFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 400));
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShake);
		GetWorld()->GetTimerManager().SetTimer(MeteorTimer, FTimerDelegate::CreateLambda([&]()->void {
			MagmaVFXComponent->Deactivate();
			EarthVFXComponent->Deactivate();;
			Destroy();
			}), 2.5f, false);
		}), 3.1f, false);
	FTimerHandle ExplosionTimer;
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimer, FTimerDelegate::CreateLambda([&]()->void {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MeteorSound, GetActorLocation());
		}), 2.6f, false);
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 1400)); //
}

void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dir = FVector::DownVector;
	FVector newLoc = GetActorLocation() + dir * METEORSPEED * DeltaTime;
	SetActorLocation(newLoc);
}

void AMeteor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto hitAct = Cast<APUPlayer>(OtherActor);
	// 플레이어와 충돌시 콜리전이 사라지기 때문에 반드시 프랙처 오브젝트와의 조건을 먼저 확인해야한다.
	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Dir = OtherComp->GetComponentLocation() - GetActorLocation();
		FVector Force = Dir * 3000;
		OtherComp->AddForce(Force);
	}
	if (hitAct != nullptr)
	{
		hitAct->OnHitEvent(METEORDAMAGE);
		// 플레이어가 메테오와 닿아서 피격당할시 콜리전을 비활성화하여 연속적인 피격을 방지해야한다.
		coll->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


