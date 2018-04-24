////////////////////////////////////////////////////////////////////////////////
// Filename: trackclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TRACKCLASS_H_
#define _TRACKCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>
#include <time.h>
#include "terrainclass.h"

/////////////
// GLOBALS //
/////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: TrackClass
////////////////////////////////////////////////////////////////////////////////
struct MapNodeType
{
	D3DXVECTOR3 centerPoint;
	int neighbours[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	int index;
	float distanceFromParent;
	float distanceToGoal;
	float distanceEstimate;
	int parent = -1;
	bool isFlat;
};

struct CheckPointBox
{
public:
	CheckPointBox(D3DXVECTOR3 BL, D3DXVECTOR3 TL, D3DXVECTOR3 TR, D3DXVECTOR3 BR) {
		bottomLeft = D3DXVECTOR3(BL.x, 0.0f, BL.z);
		topLeft = D3DXVECTOR3(TL.x, 0.0f, TL.z);
		topRight = D3DXVECTOR3(TR.x, 0.0f, TR.z);
		bottomRight = D3DXVECTOR3(BR.x, 0.0f, BR.z);
	};

	D3DXVECTOR3 bottomLeft, topLeft, topRight, bottomRight;
};

class TrackClass
{
public:
	TrackClass();
	TrackClass(const TrackClass&);
	~TrackClass();

	bool InitializeTrack(ID3D11Device* device, TerrainClass* terrain_in, int terrainWidth, int terrainHeight, WCHAR* textureFilename);
	void Shutdown();

	GeometryType* m_model;

	std::vector<D3DXVECTOR3> trackPoints;
	std::vector<D3DXVECTOR3> nodesOnPath;
	bool Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	TextureClass* GetTexture();

	D3DXVECTOR3 playerStartPos;
	std::vector<D3DXVECTOR3> opponentRacingLine;
	D3DXVECTOR3 carsStartDirection;

	VertexType* vertices;
	int m_vertexCount;
	void DeleteVertices();

	bool GenerateTrack();
	bool DeleteTrack();

	int attempts = 0;
	float trackLength = 0.0f;

	std::vector<CheckPointBox> checkPoints;
	int noOfCheckpoints = 4; //Including finish line

private:
	int startNode = 0;
	int endNode;
	std::vector<int> openList;
	std::vector<int> closedList;
	float DistanceToStart(int node);
	bool ExplorePath(int currentNode);
	bool BuildPath(int endNode);
	D3DXVECTOR3 CalculateNormal(D3DXVECTOR3 triPoint1, D3DXVECTOR3 triPoint2, D3DXVECTOR3 triPoint3);
	float nodeLength;

	int index;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	std::vector<MapNodeType> nodes;
	bool InitializeBuffers(ID3D11Device*);
	TerrainClass* m_Terrain;
	TextureClass* m_Texture;
	bool LoadTexture(ID3D11Device* device, WCHAR* textureFilename);

	ID3D11Device* m_Device;
	int m_terrainWidth;
	int m_terrainHeight;

	void ShutdownBuffers();
	bool trackGenerated = false;

	int trackpointsBetweenCheckpoints;
};

#endif
