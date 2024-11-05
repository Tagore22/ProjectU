#include "GhostTail.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

AGhostTail::AGhostTail()
{
	PrimaryActorTick.bCanEverTick = true;
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	PoseableMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PoseMesh(TEXT("SkeletalMesh'/Game/Character/Player/Mesh/Eve_By_J_Gonzales.Eve_By_J_Gonzales'"));
	if (SK_PoseMesh.Succeeded())
	{
		PoseableMesh->SetSkeletalMesh(SK_PoseMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance> M_GhostTail(TEXT("MaterialInstanceConstant'/Game/Materials/Assert_Skill_VFX_Inst.Assert_Skill_VFX_Inst'"));
	if (M_GhostTail.Succeeded())
	{
		GhostMaterial = M_GhostTail.Object;
	}
}

void AGhostTail::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGhostTail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSpawned)
	{
		FadeCountDown -= DeltaTime;
		for (int i = 0; i < Materials.Num(); i++)
		{
			Materials[i]->SetScalarParameterValue("Opacity", FadeCountDown / FadeOutTime);
		}
		if (FadeCountDown < 0)
		{
			Destroy();
		}
	}
}

void AGhostTail::MakeGhostTail(USkeletalMeshComponent* Pawn)
{
	PoseableMesh->CopyPoseFromSkeletalComponent(Pawn);
	TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();

	for (int i = 0; i < Mats.Num(); ++i)
	{
		Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial));
		PoseableMesh->SetMaterial(i, Materials[i]);
	}
	FadeCountDown = FadeOutTime;
	IsSpawned = true;
}

