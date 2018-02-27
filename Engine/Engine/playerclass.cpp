////////////////////////////////////////////////////////////////////////////////
// Filename: playerclass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "playerclass.h"

PlayerClass::PlayerClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

PlayerClass::PlayerClass(const PlayerClass &)
{
}

PlayerClass::~PlayerClass()
{
}

bool PlayerClass::Initialize(TerrainClass* terrain_in)
{
	m_Terrain = terrain_in;
	return true;
}

bool PlayerClass::Update()
{

	return true;
}

bool PlayerClass::Shutdown()
{
	m_Terrain = 0;
	return true;
}

void PlayerClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void PlayerClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

float PlayerClass::FindSurfaceLevel()
{
	//Get position of player
	D3DXVECTOR3 myPosition = GetPosition();
	D3DXVECTOR3 downwardVector = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	D3DXVECTOR3 upwardVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//Find which triangles are within range - for each vertex in the terrain...
	for (int i = 0; i < m_Terrain->GetVertexCount(); i+=3) {
		//Get vertex position
		D3DXVECTOR3 vertexPosition = m_Terrain->GetVertexPosition(i);

		//Find distance between player and vertex
		D3DXVECTOR3 vectorBetween;
		D3DXVec3Subtract(&vectorBetween, &myPosition, &vertexPosition);
		float distanceFromVertex = D3DXVec3Length(&vectorBetween);

		//if the vertex is within collision checking range...
		if (distanceFromVertex < 5.0f) {
			//Define the triange to test
			D3DXVECTOR3 triPoint2 = m_Terrain->GetVertexPosition(i);
			D3DXVECTOR3 triPoint1 = m_Terrain->GetVertexPosition(i + 1);
			D3DXVECTOR3 triPoint0 = m_Terrain->GetVertexPosition(i + 2);

			//Create output variables for function
			float u;
			float v;
			float distFromTri;

			//Cast ray downwards and recieve barycentric co-ordinates for collision
			if (D3DXIntersectTri(&triPoint0, &triPoint1, &triPoint2, &myPosition, &downwardVector, &u, &v, &distFromTri)) {
				//Use barycentric co-ordinates to find cartesian co-ordinates for collision
				D3DXVECTOR3 result = triPoint0 + (u * (triPoint1 - triPoint0)) + (v * (triPoint2 - triPoint0));

				//Return y-value
				return result.y;
			}
			//If no downward collision is found, the player may have moved below terrain, so check upwards
			else if (D3DXIntersectTri(&triPoint0, &triPoint1, &triPoint2, &myPosition, &upwardVector, &u, &v, &distFromTri)) {
				//Use barycentric co-ordinates to find cartesian co-ordinates for collision
				D3DXVECTOR3 result = triPoint0 + (u * (triPoint1 - triPoint0)) + (v * (triPoint2 - triPoint0));

				//Return y-value
				return result.y;
			}
		}
	}

	//If no collision is found, return the currently held position
	return m_positionY;
}

D3DXVECTOR3 PlayerClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 PlayerClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}
