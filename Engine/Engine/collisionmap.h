////////////////////////////////////////////////////////////////////////////////
// Filename: collisionmap.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLLISIONMAP_H_
#define _COLLISIONMAP_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include "terrainclass.h"
#include "trackclass.h"
#include "car.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: CollisionMap
////////////////////////////////////////////////////////////////////////////////
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

	bool isWithin(D3DXVECTOR3 toCheck) {
		float s = a.z * c.x - a.x * c.z + (c.z - a.z) * toCheck.x + (a.x - c.x) * toCheck.z;
		float t = a.x * b.z - a.z * b.x + (a.z - b.z) * toCheck.x + (b.x - a.x) * toCheck.z;

		if ((s < 0) != (t < 0)) {
			return false;
		}

		float A = -b.z * c.x + a.z * (c.x - b.x) + a.x * (b.z - c.z) + b.x * c.z;
		if (A < 0.0)
		{
			s = -s;
			t = -t;
			A = -A;
		}
		return s > 0 && t > 0 && (s + t) <= A;
	}
};

class CollisionMap
{
public:
	CollisionMap();
	CollisionMap(const CollisionMap&);
	~CollisionMap();

	bool Initialize(ID3D11Device* m_device, TerrainClass* terrain_in, TrackClass* track_in);
	float GetHeight(Car* car);
	bool CheckCollision(Car* car);

private:
	TerrainClass* m_Terrain;
	TrackClass* m_Racetrack;
	std::vector<Triangle> terrainCheck1;
	std::vector<Triangle> terrainCheck2;
	std::vector<Triangle> terrainCheck3;
	std::vector<Triangle> terrainCheck4;
	std::vector<Triangle> terrainCheck5;
	std::vector<Triangle> terrainCheck6;
	std::vector<Triangle> terrainCheck7;
	std::vector<Triangle> terrainCheck8;
	std::vector<Triangle> terrainCheck9;
	std::vector<Triangle> terrainCheck10;
	std::vector<Triangle> terrainCheck11;
	std::vector<Triangle> terrainCheck12;
	std::vector<Triangle> terrainCheck13;
	std::vector<Triangle> terrainCheck14;
	std::vector<Triangle> terrainCheck15;
	std::vector<Triangle> terrainCheck16;
	std::vector<Triangle> trackCheck;
};

#endif