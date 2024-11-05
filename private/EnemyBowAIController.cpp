#include "EnemyBowAIController.h"
#include "EnemyBow.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyBowAIController::AEnemyBowAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> findTree(TEXT("BehaviorTree'/Game/AI/EnemyBow_Tree.EnemyBow_Tree'"));
	if (findTree.Succeeded())
		bT = findTree.Object;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> findBlackboard(TEXT("BlackboardData'/Game/AI/EnemyBow_Blackboard.EnemyBow_Blackboard'"));
	if (findTree.Succeeded())
		bBData = findBlackboard.Object;
}