#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyBossAIController.generated.h"

UCLASS()
class PROJECTU_API AEnemyBossAIController : public AEnemyBaseAIController
{
	GENERATED_BODY()
public:
	AEnemyBossAIController();
protected:
	virtual void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus) override; // 헤더에 #include "Perception/AIPerceptionTypes.h"가 존재해야 두번째 매개변수
	// 가 등록되었음.
private:
	bool isHPBarOn = false;
};
