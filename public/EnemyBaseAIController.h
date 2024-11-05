#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyBaseAIController.generated.h"

UCLASS()
class PROJECTU_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyBaseAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual void RunAI();
	UFUNCTION(BlueprintCallable)
	void StopAI();
	void Spawned();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	TArray<class AActor*> patrolArr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackDelay", Meta = (AllowPrivateAccess = true))
	bool isAttackDelay = true; 
	bool isReady = false;
	UPROPERTY(VisibleAnywhere, Category = "BlackBoard", Meta = (AllowPrivateAccess = true))
	class UBehaviorTree* bT;
	class UBlackboardData* bBData;
	class UAISenseConfig_Sight* sightConfig;
	class UAISenseConfig_Hearing* hearingConfig;
	int32 patrolIndex = 0;
	float attackDelayTime = 0.0f;
	float curTime = 0.0f;
	const float MINATTACKDELAY = 0.5f;
	const float MAXATTACKDELAY = 2.0f;
	const float AI_SIGHT_RADIUS = 1500.f; // 적의 시야 인식 범위. 후에 따로 조정할것.
	const float AI_LOSE_SIGHT_RADIUS = 1500.f;
	const float AI_FIELD_OF_VIEW = 90.f;
	const float AI_HEARING_RANGE = 300.0f;
	const float AI_MAXAGE = 0.5f;

	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void PopPatrolLocation();
	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = true))
	FVector GetPatrolLocation();
	void CalAttackDelay(float deltaTime);
	UFUNCTION()
	virtual void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus); // 헤더에 #include "Perception/AIPerceptionTypes.h"가 존재해야 두번째 매개변수
	// 가 등록되었음.
	UFUNCTION(BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void ResetTarget();
};