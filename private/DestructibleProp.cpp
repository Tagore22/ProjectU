#include "DestructibleProp.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"

ADestructibleProp::ADestructibleProp()
{
	Coll = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Coll->SetCollisionProfileName("Barrier");
	RootComponent = Coll;

	DM = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Mesh"));
	DM->SetupAttachment(RootComponent);
	DM->SetNotifyRigidBodyCollision(true);
}

void ADestructibleProp::BeginPlay()
{
	Super::BeginPlay();
	
	DM->SetSimulatePhysics(true);
	DM->OnComponentBeginOverlap.AddDynamic(this, &ADestructibleProp::OnOverlapBegin);
}

void ADestructibleProp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FTimerHandle DMTimer;
	GetWorld()->GetTimerManager().SetTimer(DMTimer, FTimerDelegate::CreateLambda([&]()->void {
		Destroy();
		}), 2.5f, false);
}

