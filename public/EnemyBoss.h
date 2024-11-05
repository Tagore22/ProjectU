#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBoss.generated.h"

UCLASS()
class PROJECTU_API AEnemyBoss : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyBoss();
	virtual void Tick(float DeltaTime) override;
	virtual void OnDamageProcess(int32 Damage, bool IsKnockBack) override;
	void InitBossHPBar();
protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION(BlueprintCallable)
	void CastTelPo();
	UFUNCTION(BlueprintCallable)
	void CastSummon();
	UFUNCTION(BlueprintCallable)
	void CastMeteor();
	UFUNCTION(BlueprintCallable)
	void CastBolt();
	void SpawnBolt();
	void SpawnMeteor();
	void SpawnSummon();

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraSystem* TelpoSystem;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraComponent* TelpoComponent;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraSystem* SummonSystem;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UNiagaraComponent* SummonComponent;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEnemyHPBar> HPHUD;
	const float BOSS_SIGHTDISTANCE = 5000.0f;
	const int32 BOSS_DAMAGE = 12;
	const float SPAWN_LOC = 450;
	const float SPAWN_BETWEEN = 300;
	const float BOSS_HP_MULTI = 5;
	bool IsTimerON = false;
};
