#include "EnemyAXAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAXAIController::AEnemyAXAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> findTree(TEXT("BehaviorTree'/Game/AI/EnemyAX_Tree.EnemyAX_Tree'"));
	if (findTree.Succeeded())
		bT = findTree.Object;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> findBlackboard(TEXT("BlackboardData'/Game/AI/EnemyAX_Blackboard.EnemyAX_Blackboard'"));
	if (findTree.Succeeded())
		bBData = findBlackboard.Object;
}
