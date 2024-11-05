#include "EnemyBaseAIController.h"
#include "EnemyBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PUPlayer.h"
#include "Kismet/GameplayStatics.h"

AEnemyBaseAIController::AEnemyBaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	sightConfig->SightRadius = AI_SIGHT_RADIUS;
	sightConfig->LoseSightRadius = AI_LOSE_SIGHT_RADIUS;
	sightConfig->PeripheralVisionAngleDegrees = AI_FIELD_OF_VIEW;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->SetMaxAge(AI_MAXAGE);

	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
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

void AEnemyBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	attackDelayTime = FMath::RandRange(MINATTACKDELAY, MAXATTACKDELAY);
	FTimerHandle AITimer;
	GetWorld()->GetTimerManager().SetTimer(AITimer, FTimerDelegate::CreateLambda([&]()->void {
		isReady = true;
		RunAI();
		}), 0.1f, false);
}

void AEnemyBaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAttackDelay)
		CalAttackDelay(DeltaTime);
}

void AEnemyBaseAIController::RunAI()
{
	if (patrolArr.Num() == 0)
	{
		auto me = Cast<AEnemyBase>(GetPawn());
		patrolArr = me->patrolArr;
	}
	//
	if (UseBlackboard(bBData, Blackboard))
		RunBehaviorTree(bT);
}

void AEnemyBaseAIController::StopAI()
{
	auto bTC = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (bTC != nullptr)
		bTC->StopTree(EBTStopMode::Safe);
}

void AEnemyBaseAIController::PopPatrolLocation()
{
	if (patrolArr.Num() == 0)
		return;
	++patrolIndex;
	patrolIndex %= patrolArr.Num();
}

FVector AEnemyBaseAIController::GetPatrolLocation()
{
	return patrolArr[patrolIndex]->GetActorLocation();
}

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

void AEnemyBaseAIController::ResetTarget()
{
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetPawn"), nullptr);
}

void AEnemyBaseAIController::Spawned()
{
	auto me = Cast<AEnemyBase>(GetPawn());
	me->isSpawned = true;
	sightConfig->SightRadius = AI_SIGHT_RADIUS * 10;
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
	hearingConfig->HearingRange = AI_HEARING_RANGE * 1000;
	GetPerceptionComponent()->ConfigureSense(*hearingConfig);
}