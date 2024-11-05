#include "EnemySwordAIController.h"
#include "EnemySword.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemySwordAIController::AEnemySwordAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> findTree(TEXT("BehaviorTree'/Game/AI/EnemySword_Tree.EnemySword_Tree'"));
	if (findTree.Succeeded())
		bT = findTree.Object;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> findBlackboard(TEXT("BlackboardData'/Game/AI/EnemySword_Blackboard.EnemySword_Blackboard'"));
	if (findTree.Succeeded())
		bBData = findBlackboard.Object;
}