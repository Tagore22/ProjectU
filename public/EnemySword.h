#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemySword.generated.h"

UCLASS()
class PROJECTU_API AEnemySword : public AEnemyBase
{
	GENERATED_BODY()
public:
	AEnemySword();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
private:
	// EnemyAX�� ���밪��. ��� �����ڿ��� �θ�Ŭ������ �����ϴ� ����� �ٲ����Ѵ�.
	const float SWORD_WALKSPEED = 150.0f;
	const float SWORD_RUNSPEED = 600.0f;
	const float SWORD_ATTACKRANGE = 200.0f;
	const float SWORD_ATTACKRADIUS = 50.0f;
	const int32 SWORD_MINCOMBO = 1;
	const int32 SWORD_MAXCOMBO = 4;
	const float SWORD_TARGETSPEED = 75.0f;
	const int32 SWORD_DAMAGE = 22;
};
