// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTU_API UEquipmentItem : public UItem
{
	GENERATED_BODY()
	
protected:
	virtual void Use(class APUPlayer* player) override;
};

