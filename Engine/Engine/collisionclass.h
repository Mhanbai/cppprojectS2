////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLLISIONCLASS_H_
#define _COLLISIONCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// Class name: CollisionClass
////////////////////////////////////////////////////////////////////////////////
class CollisionClass
{
public:
	struct Triangle
	{
		D3DXVECTOR3 a;
		D3DXVECTOR3 b;
		D3DXVECTOR3 c;
		D3DXVECTOR3 centerPos;

		Triangle(D3DXVECTOR3 a_in, D3DXVECTOR3 b_in, D3DXVECTOR3 c_in) {
			a = a_in;
			b = b_in;
			c = c_in;
			centerPos = FindPoint(0.5f, 0.5f);
		}

		D3DXVECTOR3 FindPoint(float u, float v) {
			return a + (u * (b - a)) + (v * (c - a));
		}
	};

	CollisionClass();
	CollisionClass(const CollisionClass&);
	~CollisionClass();

	std::vector<Triangle> collisionChecklist;
	void AddToCollisionChecklist(Triangle toAdd);
};

#endif
