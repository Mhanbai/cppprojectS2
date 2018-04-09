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

bool CollisionClass::Initialize()
{
	return true;
}

void CollisionClass::AddToCollisionChecklist(Triangle toAdd)
{
	collisionChecklist.push_back(toAdd);
}
