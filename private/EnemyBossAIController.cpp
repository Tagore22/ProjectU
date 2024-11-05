#include "EnemyBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyBoss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PUPlayer.h"

AEnemyBossAIController::AEnemyBossAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> findTree(TEXT("BehaviorTree'/Game/AI/EnemyBoss_Tree.EnemyBoss_Tree'"));
	if (findTree.Succeeded())
		bT = findTree.Object;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> findBlackboard(TEXT("BlackboardData'/Game/AI/EnemyBoss_Blackboard.EnemyBoss_Blackboard'"));
	if (findTree.Succeeded())
		bBData = findBlackboard.Object;
}

void AEnemyBossAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (!isHPBarOn)
	{
		isHPBarOn = true;
		auto me = Cast<AEnemyBoss>(GetPawn());
		me->InitBossHPBar();
		auto player = Cast<APUPlayer>(Actor);
		player->WalkSound_MaxRange *= 100;
	}
	Super::OnTargetDetected(Actor, Stimulus);
}