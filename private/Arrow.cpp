#include "Arrow.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PUPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;
	coll = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	coll->SetCollisionProfileName(TEXT("Arrow"));
	RootComponent = coll;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	mesh->SetupAttachment(RootComponent);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> findMesh(TEXT("StaticMesh'/Game/Blueprints/SM_ArrowTrail.SM_ArrowTrail'"));
	if (findMesh.Succeeded())
	{
		mesh->SetStaticMesh(findMesh.Object);
		mesh->SetRelativeRotation(FRotator(90, 0, 0));
		mesh->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	}
	Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	Trail->SetupAttachment(mesh);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> findTrail(TEXT("ParticleSystem'/Game/Particles/P_Trail.P_Trail'"));
	if (findTrail.Succeeded())
	{
		Trail->SetTemplate(findTrail.Object);
	}
	ConstructorHelpers::FObjectFinder<USoundBase> findClang(TEXT("SoundWave'/Game/Sounds/ArrowClang.ArrowClang'"));
	if (findClang.Succeeded()) {
		ClangSound = findClang.Object;
	}
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

	auto target = Cast<APUPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)); 
	FRotator transRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
	SetActorRotation(FRotator(transRot.Pitch, transRot.Yaw, GetActorRotation().Roll));
	coll->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);
	SpawnLoc = GetActorLocation();
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLoc = GetActorLocation() + GetActorForwardVector() * speed * DeltaTime;
	SetActorLocation(newLoc);
	if (FVector::Dist(SpawnLoc, GetActorLocation()) > DistanceLimit)
	{
		Destroy();
	}
}

void AArrow::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto hitAct = Cast<APUPlayer>(OtherActor);
	if (hitAct != nullptr)
	{
		hitAct->OnHitEvent(Damage);
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ClangSound);
	}
	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Dir = GetActorLocation() - OtherComp->GetComponentLocation();
		FVector Force = Dir * -20000; 
		OtherComp->AddForce(Force);
	}
	Destroy();
}