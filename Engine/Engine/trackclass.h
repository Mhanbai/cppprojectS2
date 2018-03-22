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


class TrackClass
{
public:
	TrackClass();
	TrackClass(const TrackClass&);
	~TrackClass();

	bool InitializeTrack(ID3D11Device* device, TerrainClass* terrain_in, int terrainWidth, int terrainHeight);
	void Shutdown();

	GeometryType* m_model;

	std::vector<D3DXVECTOR3> trackPoints;
	std::vector<D3DXVECTOR3> nodesOnPath;
	bool Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	int startNode = 0;
	int endNode;
	std::vector<int> openList;
	std::vector<int> closedList;
	float DistanceToStart(int node);
	bool ExplorePath(int currentNode);
	void BuildPath(int endNode);

	int index;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	std::vector<MapNodeType> nodes;
	bool InitializeBuffers(ID3D11Device*);
	TerrainClass* m_Terrain;
};

#endif
