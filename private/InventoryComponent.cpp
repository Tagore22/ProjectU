#include "InventoryComponent.h"
#include "Item.h"

UInventoryComponent::UInventoryComponent()
{

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& item : defaultItems)
	{
		AddItem(item);
	}
	
}

bool UInventoryComponent::AddItem(class UItem* item)
{
	if (item == nullptr)
	{
		return false;
	}

	item->owningInventory = this;
	item->world = GetWorld();
	items.Add(item);
	// UI 업데이트.
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(class UItem* item)
{
	if (item)
	{
		item->owningInventory = nullptr;
		item->world = nullptr;
		items.RemoveSingle(item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

