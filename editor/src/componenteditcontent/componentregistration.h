#ifndef COMPONENTREGISTRATION_H
#define COMPONENTREGISTRATION_H

#include <map>
#include <memory>
#include <string>

#include "abstracteditfactory.h"

class ComponentFactory;

namespace ComponentRegistration
{
	void RegisterToEditFactory(std::map<std::string, std::unique_ptr<AbstractEditFactory>>& factories);
	void RegisterComponentFactory(ComponentFactory& factory);
}

#endif // COMPONENTREGISTRATION_H
