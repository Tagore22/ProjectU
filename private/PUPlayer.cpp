#include "PUPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "EnemyBase.h"
#include "PlayerAnim.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputTriggers.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.h"
#include "EngineUtils.h"
#include "Components/TextBlock.h"
#include "GhostTail.h"

APUPlayer::APUPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleRadius(32);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> findMesh(TEXT("SkeletalMesh'/Game/Character/Player/Mesh/Eve_By_J_Gonzales.Eve_By_J_Gonzales'"));
	if (findMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(findMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> findAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Player.ABP_Player_C'"));
	if (findAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(findAnim.Class);

	AxeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AxeMesh"));
	AxeMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindAxe(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Axe.SK_Axe'"));
	if (FindAxe.Succeeded())
	{
		AxeMesh->SetSkeletalMesh(FindAxe.Object);
		AxeMesh->SetRelativeRotation(FRotator(0, -45, 180));
		AxeMesh->SetVisibility(false);
	}
	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindSword(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Sword.SK_Sword'"));
	if (FindSword.Succeeded())
	{
		SwordMesh->SetSkeletalMesh(FindSword.Object);
		SwordMesh->SetVisibility(false);
	}
	MaceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaceMesh"));
	MaceMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindMace(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Mace.SK_Mace'"));
	if (FindAxe.Succeeded())
	{
		MaceMesh->SetSkeletalMesh(FindMace.Object);
		MaceMesh->SetRelativeRotation(FRotator(0, 0, 180));
		MaceMesh->SetVisibility(false);
	}
	DaggerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DaggerMesh"));
	DaggerMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindDagger(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Dagger_1.SK_Dagger_1'"));
	if (FindAxe.Succeeded())
	{
		DaggerMesh->SetSkeletalMesh(FindDagger.Object);
		DaggerMesh->SetRelativeLocationAndRotation(FVector(0, 0, 7), FRotator(0, 0, 180));
		DaggerMesh->SetVisibility(true);
	}
	PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	PotionMesh->SetupAttachment(GetMesh(), TEXT("PotionPosition"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FindPotion(TEXT("StaticMesh'/Game/Potion/SM_Potion.SM_Potion'"));
	if (FindPotion.Succeeded())
	{
		PotionMesh->SetStaticMesh(FindPotion.Object);
		PotionMesh->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.12f));
		PotionMesh->SetRelativeLocationAndRotation(FVector(0, 0, 5), FRotator(20, 120, 150));
		PotionMesh->SetVisibility(false);
	}

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 400;
	springArm->SetRelativeLocation(FVector(0, 0, 90));
	springArm->bUsePawnControlRotation = true;
	springArm->bEnableCameraLag = true;

	camera = CreateDefaultSubobject<UCameraComponent> (TEXT("Camera"));
	camera->SetupAttachment(springArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 1000, 0);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> findContext(TEXT("InputMappingContext'/Game/Input/InputSpaceMappingContext.InputSpaceMappingContext'"));
	if (findContext.Succeeded())
		mapContext = findContext.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> findDodgeAction(TEXT("InputAction'/Game/Input/DodgeAction.DodgeAction'"));
	if (findDodgeAction.Succeeded())
		dodgeAction = findDodgeAction.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> findRunAction(TEXT("InputAction'/Game/Input/RunAction.RunAction'"));
	if (findRunAction.Succeeded())
		runAction = findRunAction.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase> findHitSound(TEXT("SoundWave'/Game/Sounds/HitSound.HitSound'"));
	if (findHitSound.Succeeded())
		hitSound = findHitSound.Object;

	inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	DropLoc = CreateDefaultSubobject<USceneComponent>(TEXT("Loc"));
	DropLoc->SetupAttachment(RootComponent);
	DropLoc->SetRelativeLocation(FVector(60, 0, -90));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FindVFX(TEXT("NiagaraSystem'/Game/VFX/N_KnockBackSkillVFX.N_KnockBackSkillVFX'"));
	if (FindVFX.Succeeded())
	{
		VFXSystem = FindVFX.Object;
	}
	PotionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PotionEffect"));
	PotionEffect->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FindPotionEffect(TEXT("ParticleSystem'/Game/VFX/P_PotionEffect.P_PotionEffect'"));
	if (FindPotionEffect.Succeeded())
	{
		PotionEffect->SetTemplate(FindPotionEffect.Object);
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> FindCameraShake(TEXT("Blueprint'/Game/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	if (FindCameraShake.Succeeded())
	{
		CameraShake = FindCameraShake.Class;
	}
}

void APUPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WALKSPEED;
	anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	if (anim != nullptr)
	{
		anim->OnMontageEnded.AddDynamic(this, &APUPlayer::OnAttackMontageEnd);
		anim->attackEnd.AddLambda([this]()->void
			{
				if (!canNextCombo)
					anim->AttackStop();
				canNextCombo = false;
			});
		anim->backStabEnd.AddUObject(this, &APUPlayer::ReturnState);
		anim->dodgeEnd.AddUObject(this, &APUPlayer::ReturnState);
		anim->damagedEnd.AddUObject(this, &APUPlayer::ReturnState);
		anim->knockBackStart.AddUObject(this, &APUPlayer::KnockBackOn);
		anim->knockBackSkillEnd.AddLambda([this]()->void
			{
				ReturnState();
				VisibleWeapon();
				if (VFXComponent != nullptr)
				{
					VFXComponent->Deactivate();
				}
			});
		anim->potionEnd.AddLambda([this]()->void
			{
				ReturnState();
				VisibleWeapon();
				PotionMesh->SetVisibility(false);
				PotionEffect->SetVisibility(false);
			});
		anim->healEnd.AddLambda([this]()->void
			{
				--PotionCount;
				HUDRef->PotionCount->SetText(FText::AsNumber(PotionCount));
				hp = FMath::Min(hp + 50, 100);
			});
		anim->attackHitCheck.AddLambda([this]()->void
			{
				AttackCheck();
				stamina -= FMath::Min(CONSUME_STA, stamina);
			});
		anim->assertCheck.AddLambda([this]()->void
			{
				AttackCheck();
			});
		anim->assertEnd.AddLambda([this]()->void
			{
				ReturnState();
			});
		anim->spawnGhostTail.AddLambda([this]()->void
			{
				SpawnGhostTail();
			});
	}
	EndCombo();

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(mapContext, 0);
	}
	if (GameHUD != nullptr)
	{
		HUDRef = CreateWidget<UMainWidget>(GetWorld(), GameHUD);
		if (HUDRef != nullptr)
		{
			HUDRef->AddToViewport();
			HUDRef->PotionCount->SetText(FText::AsNumber(PotionCount));
		}
	}
	PotionEffect->SetVisibility(false);
}

void APUPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	f_Vec = camera->GetForwardVector();
	r_Vec = camera->GetRightVector();
	if ((mState == EPlayerState::Idle || mState == EPlayerState::Damage || mState == EPlayerState::Potion) && stamina < MAX_STA)
	{
		stamina += FMath::Min(RECOVERY_STA, MAX_STA - stamina);
	}
	else if (mState == EPlayerState::Run)
	{
		stamina -= FMath::Min(0.1f, stamina);
	}
	if (stamina == 0)
	{
		EndRun();
	}

	if (lockedTarget != nullptr)
	{
		LookTargetCamera(DeltaTime);
		if(mState != EPlayerState::Run && mState != EPlayerState::Dodge)
			LookTarget(DeltaTime);
		if (!lockedTarget->isAlive || FVector::Dist(lockedTarget->GetActorLocation(), GetActorLocation()) > LOCKONRANGE)
			LockOnTarget();
	}
}

void APUPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APUPlayer::MoveVertical);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APUPlayer::MoveHorizontal);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APUPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APUPlayer::Turn);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &APUPlayer::Attack);
	PlayerInputComponent->BindAction(TEXT("LockOn"), IE_Pressed, this, &APUPlayer::LockOnTarget);
	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Pressed, this, &APUPlayer::PickupItems);
	PlayerInputComponent->BindAction(TEXT("KnockBackSkill"), IE_Pressed, this, &APUPlayer::KnockBackSkill);
	PlayerInputComponent->BindAction(TEXT("Potion"), IE_Pressed, this, &APUPlayer::DrinkPotion);
	PlayerInputComponent->BindAction(TEXT("Assert"), IE_Pressed, this, &APUPlayer::UseAssertSkill);
	PlayerInputComponent->BindAction(TEXT("ToggleInven"), IE_Pressed, this, &APUPlayer::ToggleInventory).bExecuteWhenPaused = true;

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(dodgeAction, ETriggerEvent::Triggered, this, &APUPlayer::SpaceAction);
		enhancedInputComponent->BindAction(runAction, ETriggerEvent::Triggered, this, &APUPlayer::StartRun);
		enhancedInputComponent->BindAction(runAction, ETriggerEvent::Completed, this, &APUPlayer::EndRun);
	}
}

void APUPlayer::MoveVertical(float value)
{
	curDir.X = value;
	if (mState == EPlayerState::Attack || mState == EPlayerState::Damage || mState == EPlayerState::Dodge)
		return;
	AddMovementInput(camera->GetForwardVector(), value);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, WalkSound_MaxRange);
}

void APUPlayer::MoveHorizontal(float value)
{
	curDir.Y = value;
	if (mState == EPlayerState::Attack || mState == EPlayerState::Damage || mState == EPlayerState::Dodge)
		return;
	AddMovementInput(camera->GetRightVector(), value);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, WalkSound_MaxRange);
}

void APUPlayer::LookUp(float value)
{
	if(lockedTarget != nullptr)
		return;
	AddControllerPitchInput(value);
}

void APUPlayer::Turn(float value)
{
	if (lockedTarget != nullptr)
		return;
	AddControllerYawInput(value);
}

void APUPlayer::Attack()
{
	if (mState == EPlayerState::Damage || mState == EPlayerState::Dodge || mState == EPlayerState::BackStab || stamina == 0
		|| mState == EPlayerState::KnockBackSkill || mState == EPlayerState::Potion)
		return;
	if (mState == EPlayerState::Attack)
	{
		canNextCombo = true;
	}
	else
	{
		if (mState == EPlayerState::Run)
		{
			PlayMontage(TEXT("MoveAttack"));
		}
		else
		{
			FString AttackName = FString::Printf(TEXT("Attack%d"), WeaponNum);
			PlayMontage(FName(*AttackName));
		}
		mState = EPlayerState::Attack;
		anim->mState = mState;
	}
}

void APUPlayer::AttackCheck()
{	
	TArray<struct FHitResult> OutHits;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * WEAPONRANGE[WeaponNum], 
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(WEAPONRADIUS[WeaponNum]),
		params);

	// 공격 범위용 트레이스.
	/*FVector traceVec = GetActorForwardVector() * WEAPONRANGE[WeaponNum];
	FVector center = GetActorLocation() + traceVec * 0.5;
	float halfHeight = WEAPONRANGE[WeaponNum] * 0.5 + WEAPONRADIUS[WeaponNum];
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bHit ? FColor::Green : FColor::Red;
	float debugLifeTime = 5;

	DrawDebugCapsule(GetWorld(),
		center,
		halfHeight,
		WEAPONRADIUS[WeaponNum],
		capsuleRot,
		drawColor,
		false,
		debugLifeTime);*/
	if (bHit)
	{
		for (auto& hitResult : OutHits)
		{
			auto target = hitResult.GetActor();
			if (target->GetName().Contains("Enemy"))
			{
				auto enemy = Cast<AEnemyBase>(target);
				if (enemy != lockedTarget)
				{
					enemy->TimerHPBar();
				}
				int32 Damage = FMath::RandRange(WEAPONDAMAGE[WeaponNum], WEAPONDAMAGE[WeaponNum] + 50);
				enemy->OnDamageProcess(Damage, false);
				UGameplayStatics::PlaySound2D(GetWorld(), hitSound);
				// 여기서 타임스탑.
				TimeStopEnemy = enemy;
				TimeStop();
			}
		}
	}
}

void APUPlayer::EndCombo()
{
	if (mState == EPlayerState::Attack)
		ReturnState();
	canNextCombo = false;
}

void APUPlayer::StartRun()
{
	if (mState == EPlayerState::Attack || mState == EPlayerState::Dodge || mState == EPlayerState::Damage
		|| mState == EPlayerState::KnockBackSkill || mState == EPlayerState::Potion)
		return;

	GetCharacterMovement()->MaxWalkSpeed = RUNSPEED;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	mState = EPlayerState::Run;
	anim->mState = mState;
}

void APUPlayer::EndRun()
{
	if (mState != EPlayerState::Run)
		return;
	GetCharacterMovement()->MaxWalkSpeed = WALKSPEED;
	mState = EPlayerState::Idle;
	anim->mState = mState;
	GetCharacterMovement()->bOrientRotationToMovement = lockedTarget != nullptr ? false : true;
}

void APUPlayer::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	EndCombo();
}

void APUPlayer::Dodge()
{
	if (mState == EPlayerState::Attack || mState == EPlayerState::Damage || mState == EPlayerState::Dodge || stamina == 0
		|| mState == EPlayerState::KnockBackSkill || mState == EPlayerState::BackStab || mState == EPlayerState::Potion)
		return;
	stamina -= FMath::Min(CONSUME_STA, stamina);
	if (curDir != FVector::ZeroVector)
	{
		FVector dodgeVec = FTransform(FRotator(0, GetControlRotation().Yaw, 0)).TransformVector(curDir);
		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), dodgeVec + GetActorLocation());
		SetActorRotation(FRotator(0, lookAtRotation.Yaw, 0));
	}
	mState = EPlayerState::Dodge;
	anim->mState = mState;
	PlayMontage(TEXT("Dodge"));
}

void APUPlayer::OnHitEvent(int32 damage)
{
	if (mState == EPlayerState::Dodge)
		return;
	if(VFXComponent != nullptr)
		VFXComponent->Deactivate();
	VisibleWeapon();
	PotionMesh->SetVisibility(false);
	PotionEffect->SetVisibility(false);
	mState = EPlayerState::Damage;
	anim->mState = mState;
	PlayMontage(TEXT("Damage"));
	hp = FMath::Max(0, hp - damage);
	UGameplayStatics::PlaySound2D(GetWorld(), hitSound);
}

void APUPlayer::ReturnState()
{
	GetCharacterMovement()->bOrientRotationToMovement = lockedTarget != nullptr ? false : true;
	GetCharacterMovement()->MaxWalkSpeed = WALKSPEED;
	mState = EPlayerState::Idle;
	anim->mState = mState;	
}

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
		for (int i = 0; i < 120; i += 5)
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

void APUPlayer::LookTargetCamera(float deltaSeconds) const
{
	FVector lockedOnLocation = lockedTarget->GetActorLocation();
	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), lockedOnLocation);
	FRotator interpRotation = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(), lookAtRotation, deltaSeconds, 10.f);
	GetController()->SetControlRotation(FRotator(interpRotation.Pitch, interpRotation.Yaw, GetController()->GetControlRotation().Roll));
}

void APUPlayer::LookTarget(float deltaSeconds) const
{
	const FRotator PawnInterpRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), FRotator(GetActorRotation().Pitch, GetControlRotation().Yaw, GetActorRotation().Roll), deltaSeconds, 10.f);
	GetController()->GetPawn()->SetActorRotation(PawnInterpRotation);
}

void APUPlayer::SpaceAction()
{
	if (mState == EPlayerState::Dodge)
		return;
	if (curDir == FVector::ZeroVector)
	{
		BackStab();
	}
	else
	{
		Dodge();
	}
}

void APUPlayer::PlayMontage(FName sectionName)
{
	PlayAnimMontage(anim->playerMontage, 1.0, sectionName);
}

void APUPlayer::BackStab()
{
	if (mState == EPlayerState::Attack || mState == EPlayerState::Damage || mState == EPlayerState::BackStab || stamina == 0
		|| mState == EPlayerState::KnockBackSkill)
		return;
	stamina -= FMath::Min(CONSUME_STA, stamina);
	mState = EPlayerState::BackStab;
	anim->mState = mState;
	PlayMontage(TEXT("BackStab"));
}

void APUPlayer::PickupItems()
{
	if (HUDRef->Inventory.Num() < INVENCAP)
	{
		PickupItemDel.Broadcast();
	}
}

void APUPlayer::PlusHP(int32 Num)
{
	hp += Num;
}

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

void APUPlayer::EnableMouseCursor()
{
	auto PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerCon->SetInputMode(FInputModeGameAndUI());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void APUPlayer::DisableMouseCursor()
{
	auto PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerCon->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void APUPlayer::DropItem_Implementation(class AActor* ItemToDrop)
{
	if (ItemToDrop != nullptr)
	{
		ItemToDrop->SetActorHiddenInGame(false);
		ItemToDrop->SetActorEnableCollision(true);
		FTransform Loc = DropLoc->GetComponentTransform();
		Loc.SetScale3D(FVector(8, 8, 8)); 
		ItemToDrop->SetActorTransform(Loc);
	}
}

void APUPlayer::TimeStop()
{
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()->void {
		TimeStopEnemy->CustomTimeDilation = 0.0f;
		CustomTimeDilation = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()->void {
			TimeStopEnemy->CustomTimeDilation = 1.0f;
			CustomTimeDilation = 1.0f;
			TimeStopEnemy = nullptr;
			}), 0.15f, false);
		}), 0.2f, false);
}

void APUPlayer::KnockBackSkill()
{
	if (mState == EPlayerState::Attack || mState == EPlayerState::Damage || mState == EPlayerState::Dodge 
		|| mState == EPlayerState::KnockBackSkill || mState == EPlayerState::BackStab || mState == EPlayerState::Potion || Mana == 0)
		return;

	UnVisibleWeapon();
	mState = EPlayerState::KnockBackSkill;
	anim->mState = mState;
	PlayMontage(TEXT("KnockBackSkill"));
	VFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXSystem, GetActorLocation());
}

void APUPlayer::VisibleWeapon()
{
	if (WeaponNum == 1)
	{
		AxeMesh->SetVisibility(true);
	}
	else if (WeaponNum == 2)
	{
		SwordMesh->SetVisibility(true);
	}
	else if (WeaponNum == 3)
	{
		MaceMesh->SetVisibility(true);
	}
	else if (WeaponNum == 4)
	{
		DaggerMesh->SetVisibility(true);
	}
}

void APUPlayer::UnVisibleWeapon()
{
	if (WeaponNum == 1)
	{
		AxeMesh->SetVisibility(false);
	}
	else if (WeaponNum == 2)
	{
		SwordMesh->SetVisibility(false);
	}
	else if (WeaponNum == 3)
	{
		MaceMesh->SetVisibility(false);
	}
	else if (WeaponNum == 4)
	{
		DaggerMesh->SetVisibility(false);
	}
}

void APUPlayer::KnockBackOn()
{
	Mana -= FMath::Min(CONSUME_MANA, Mana);
	auto Con = GetWorld()->GetFirstPlayerController();
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShake);

	for (TActorIterator<AEnemyBase> enemy(GetWorld()); enemy; ++enemy)
	{
		if (enemy->GetName().Contains(TEXT("Enemy")) && enemy->isAlive)
		{
			FVector from = GetActorLocation();
			FVector to = enemy->GetActorLocation();
			if (FVector::Distance(from, to) <= KNOCKBACK_RANGE)
			{
				FVector dir = to - from;
				dir.Normalize();
				int32 Damage = FMath::RandRange(KNOCKBACK_Damage, KNOCKBACK_Damage + 10);
				enemy->OnDamageProcess(Damage, true);
				if (*enemy != lockedTarget)
				{
					enemy->TimerHPBar();
				}
				if (enemy->isAlive && !enemy->GetName().Contains(TEXT("Boss")))
				{
					enemy->LaunchCharacter(FVector(dir.X * KNOCKBACK_FORCE, dir.Y * KNOCKBACK_FORCE, 400), false, false);
				}
			}
		}
	}
}

void APUPlayer::DrinkPotion()
{
	if (mState != EPlayerState::Idle || PotionCount <= 0)
		return;

	mState = EPlayerState::Potion;
	PotionMesh->SetVisibility(true);
	PotionEffect->SetVisibility(true);
	UnVisibleWeapon();
	PlayAnimMontage(anim->PotionMontage, 1.0, TEXT("Potion"));	
}

void APUPlayer::UseAssertSkill()
{
	if (mState == EPlayerState::Attack || mState == EPlayerState::Damage || mState == EPlayerState::Dodge
		|| mState == EPlayerState::KnockBackSkill || mState == EPlayerState::BackStab || mState == EPlayerState::Potion || Mana == 0)
		return;

	Mana -= FMath::Min(CONSUME_ASSERT_MANA, Mana);
	stamina -= FMath::Min(CONSUME_STA, stamina);
	mState = EPlayerState::Attack;
	anim->mState = mState;
	PlayMontage(TEXT("Skill2"));
}

void APUPlayer::SpawnGhostTail()
{
	FVector pos = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 90);
	FRotator rot = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw - 90, GetActorRotation().Roll);
	auto ghost = Cast<AGhostTail>(GetWorld()->SpawnActor<AGhostTail>(AGhostTail::StaticClass(), pos, rot));
	if (ghost != nullptr)
		ghost->MakeGhostTail(GetMesh());
}
