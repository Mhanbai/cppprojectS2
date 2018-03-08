////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PLAYERCLASS_H_
#define _PLAYERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "terrainclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class PlayerClass
{
public:
	PlayerClass();
	PlayerClass(const PlayerClass&);
	~PlayerClass();

	bool Initialize(TerrainClass* terrain_in);
	bool Update();
	bool Shutdown();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	float FindSurfaceLevel();
	float FindClosestTriangle();

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	TerrainClass* m_Terrain;
	int terrainArraySize;

	float yPosOffset = 1.0f;
};

#endif
