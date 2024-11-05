#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyAX.generated.h"

UCLASS()
class PROJECTU_API AEnemyAX : public AEnemyBase
{
	GENERATED_BODY()
public:
	AEnemyAX();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
private:
	// EnemyAX의 전용값들. 모두 생성자에서 부모클래스에 대응하는 값들로 바뀌어야한다.
	const float AX_WALKSPEED = 150.0f; 
	const float AX_RUNSPEED = 600.0f;
	const float AX_ATTACKRANGE = 200.0f;
	const float AX_ATTACKRADIUS = 50.0f;
	const int32 AX_MINCOMBO = 1;
	const int32 AX_MAXCOMBO = 4;
	const float AX_TARGETSPEED = 75.0f;
	const int32 AX_DAMAGE = 27;
};
