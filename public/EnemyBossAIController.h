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
	virtual void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus) override; // ����� #include "Perception/AIPerceptionTypes.h"�� �����ؾ� �ι�° �Ű�����
	// �� ��ϵǾ���.
private:
	bool isHPBarOn = false;
};
