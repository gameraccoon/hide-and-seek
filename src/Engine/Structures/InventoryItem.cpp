#include "InventoryItem.h"


InventoryItem::InventoryItem(void)
{
	bEquipped = false;
}

InventoryItem::~InventoryItem(void)
{
}

bool InventoryItem::isEquipped()
{
	return this->bEquipped;
}

void InventoryItem::setEquipped(bool equipped)
{
	this->bEquipped = equipped;
}