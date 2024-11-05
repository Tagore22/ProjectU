#include "Portal.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


APortal::APortal()
{
	coll = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = coll;
	coll->SetSphereRadius(200.0f);
	VFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX"));
	VFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FindVFX(TEXT("ParticleSystem'/Game/VFX/P_Potral.P_Potral'"));
	if (FindVFX.Succeeded())
	{
		VFX->SetTemplate(FindVFX.Object);
	}
}

void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	coll->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto hitAct = Cast<APUPlayer>(OtherActor);
	if (hitAct != nullptr)
	{
		// ���⼭ ���������� �̵��Ѵ�.
		UGameplayStatics::OpenLevel(GetWorld(), "BossMap");
	}
}

