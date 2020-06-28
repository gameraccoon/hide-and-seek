#pragma once

class BaseComponent;

class AbstractComponentImguiWidget
{
public:
	virtual void update(BaseComponent* component) = 0;
};
