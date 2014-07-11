#ifndef INVENTORY_ITEM_H
#define INVENTORY_ITEM_H

class InventoryItem
{
public:
	InventoryItem(void);
	virtual ~InventoryItem(void);
	virtual bool isEquipped();
	virtual void setEquipped(bool equipped);
private:
	bool bEquipped;
};

#endif
