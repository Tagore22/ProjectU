#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionInterface.h"
#include "TemplateItem.generated.h"

UCLASS()
class PROJECTU_API ATemplateItem : public AActor, public IActionInterface
{
	GENERATED_BODY()
public:	
	ATemplateItem();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void UseItem_Implementation() override;
	UFUNCTION(BlueprintCallable)
	void PickupEvent();
	void UseFood();
	UFUNCTION(BlueprintCallable)
	void UseEquipment(int32 Num);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Static Mesh")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* Trigger;
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	class UWidgetComponent* Widget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Isinrange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	struct FInvenStruct Iteminfo;
	UPROPERTY(EditAnywhere)
	int32 Addhealth = 35;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WeaponNumber;
	UPROPERTY(EditAnywhere)
	bool IsFood;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) 
	class UPickText* PickupRef;
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UPickText> PickupWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class APUPlayer* PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* CustomImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText CustomPickupText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText CustomActionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* CustomStaticMesh;
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UParticleSystemComponent* PillarOfLight;
};
