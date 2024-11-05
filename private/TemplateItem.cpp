#include "TemplateItem.h"
#include "Components/SphereComponent.h"
#include "PickText.h"
#include "PUPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "MainWidget.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h" 

ATemplateItem::ATemplateItem()
{
	PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	RootComponent = Trigger;
	Trigger->SetWorldScale3D(FVector(8.0f, 8.0f, 8.0f));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Trigger);
	Mesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetupAttachment(RootComponent);
	PillarOfLight = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PillarOfLight"));
	PillarOfLight->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FindPillarOfLight(TEXT("ParticleSystem'/Game/VFX/P_Weapon_Pillar.P_Weapon_Pillar'"));
	if (FindPillarOfLight.Succeeded())
	{
		PillarOfLight->SetTemplate(FindPillarOfLight.Object);
		PillarOfLight->SetWorldScale3D(FVector(0.15f, 0.15f, 0.15f));
	}
}

void ATemplateItem::BeginPlay()
{
	Super::BeginPlay();

	PickupRef = Cast<UPickText>(Widget->GetUserWidgetObject());
	PickupRef->PickupActor = Iteminfo.Item;
	PickupRef->PickupText = Iteminfo.PickupText;
	PlayerRef = Cast<APUPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATemplateItem::OnBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ATemplateItem::OnEndOverlap);
}

void ATemplateItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATemplateItem::OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<APUPlayer>(OtherActor);
	if (player != nullptr)
	{
		Widget->SetVisibility(true);
		Isinrange = true;
		Mesh->SetRenderCustomDepth(true);
	}
}

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

void ATemplateItem::UseItem_Implementation()
{
	if (IsFood)
	{
		UseFood();
	}
	else
	{
		UseEquipment(WeaponNumber);
	}
}

void ATemplateItem::PickupEvent()
{
	if (GetActorEnableCollision() && Isinrange)
	{
		PlayerRef->HUDRef->Inventory.Add(Iteminfo);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

void ATemplateItem::UseFood()
{
	PlayerRef->PlusHP(Addhealth);
	Destroy();
	PlayerRef->PrevIsFood = true;
}

void ATemplateItem::UseEquipment(int32 Num)
{
	int PrevNum = PlayerRef->WeaponNum;
	if (PrevNum == 1)
	{
		PlayerRef->AxeMesh->SetVisibility(false);
	}
	else if (PrevNum == 2)
	{
		PlayerRef->SwordMesh->SetVisibility(false);
	}
	else if (PrevNum == 3)
	{
		PlayerRef->MaceMesh->SetVisibility(false);
	}
	else if (PrevNum == 4)
	{
		PlayerRef->DaggerMesh->SetVisibility(false);
	}

	if (Num == 1)
	{
		PlayerRef->AxeMesh->SetVisibility(true);
	}
	else if (Num == 2)
	{
		PlayerRef->SwordMesh->SetVisibility(true);
	}
	else if (Num == 3)
	{
		PlayerRef->MaceMesh->SetVisibility(true);
	}
	else if (Num == 4)
	{
		PlayerRef->DaggerMesh->SetVisibility(true);
	}
	PlayerRef->WeaponNum = Num;
	PlayerRef->PrevIsFood = false;
}


