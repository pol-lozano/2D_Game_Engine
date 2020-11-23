#include "ECS.h"

//Add entity to group
void Entity::addGroup(Group mGroup)
{
	groupBitset[mGroup] = true;
	manager.addToGroup(this, mGroup);
}
