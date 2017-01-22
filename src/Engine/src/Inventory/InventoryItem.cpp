#include "Inventory/InventoryItem.h"


InventoryItem::InventoryItem()
{
	mIsEquipped = false;
}

InventoryItem::~InventoryItem()
{
}

bool InventoryItem::isEquipped()
{
	return mIsEquipped;
}

void InventoryItem::setEquipped(bool equipped)
{
	mIsEquipped = equipped;
}
