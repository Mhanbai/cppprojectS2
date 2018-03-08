#include "collisionclass.h"

CollisionClass::CollisionClass()
{
}

CollisionClass::CollisionClass(const CollisionClass &)
{
}

CollisionClass::~CollisionClass()
{
}

void CollisionClass::AddToCollisionChecklist(Triangle toAdd)
{
	collisionChecklist.push_back(toAdd);
}
