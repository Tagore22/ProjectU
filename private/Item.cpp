#include "Item.h"

UItem::UItem()
{
	itemDisplayName = FText::FromString("Item");
	useActionText = FText::FromString("Use");
}

void UItem::Use(class APUPlayer* player)
{

}

UWorld* UItem::GetWorld() const
{
	return world;
}