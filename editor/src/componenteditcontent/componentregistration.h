#ifndef COMPONENTREGISTRATION_H
#define COMPONENTREGISTRATION_H

#include <map>
#include <memory>
#include <string>

#include "abstracteditfactory.h"

class ComponentFactory;

namespace ComponentRegistration
{
	void RegisterToEditFactory(std::map<StringID, std::unique_ptr<AbstractEditFactory>>& factories);
}

#endif // COMPONENTREGISTRATION_H
