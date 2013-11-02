#include "InventoryItem.h"


InventoryItem::InventoryItem(void)
{
	bEquipped = false;
}

InventoryItem::~InventoryItem(void)
{
}

bool InventoryItem::IsEquipped()
{
	return bEquipped;
}

void InventoryItem::SetEquipped(bool equipped)
{
	bEquipped = equipped;
}