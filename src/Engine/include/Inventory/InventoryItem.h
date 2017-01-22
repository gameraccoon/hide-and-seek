#ifndef INVENTORY_ITEM_H
#define INVENTORY_ITEM_H

class InventoryItem
{
public:
	InventoryItem();
	virtual ~InventoryItem();
	virtual bool isEquipped();
	virtual void setEquipped(bool equipped);

private:
	bool mIsEquipped;
};

#endif
