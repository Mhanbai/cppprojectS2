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

/////////////
// GLOBALS //
/////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: TrackClass
////////////////////////////////////////////////////////////////////////////////
struct VertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

struct GeometryType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

struct MapNodeType
{
	D3DXVECTOR3 bottomLeft;
	D3DXVECTOR3 topRight;
	int neighbours[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	int parent;
	int index;
	bool isFlat;
	D3DXVECTOR3 GetCenterPoint() {
		return (bottomLeft + topRight) * 0.5f;
	};
};

class TrackClass
{
public:
	TrackClass();
	TrackClass(const TrackClass&);
	~TrackClass();

	bool InitializeTrack(GeometryType* heightMap, int terrainWidth, int terrainHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	GeometryType* m_model;

	std::vector<D3DXVECTOR3> trackPoints;
	void Render(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	std::vector<MapNodeType> nodes;
	int CreateTrack(int node);
	bool InitializeBuffers(ID3D11Device*);
	std::vector<int> nodesOnPath;
};

#endif
