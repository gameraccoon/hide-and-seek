#pragma once

class BaseComponent;

class AbstractComponentImguiWidget
{
public:
	virtual ~AbstractComponentImguiWidget() = default;

	virtual void update(BaseComponent* component) = 0;
};
