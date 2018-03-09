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

/////////////
// GLOBALS //
/////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: TrackClass
////////////////////////////////////////////////////////////////////////////////
struct HeightMapType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

struct MapNodeType
{
	D3DXVECTOR3 bottomLeft;
	D3DXVECTOR3 bottomRight;
	D3DXVECTOR3 topLeft;
	D3DXVECTOR3 topRight;
	bool isFlat;
};

class TrackClass
{
public:
	TrackClass();
	TrackClass(const TrackClass&);
	~TrackClass();

	bool InitializeTrack(HeightMapType* heightMap, int terrainWidth, int terrainHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

private:
	std::vector<MapNodeType> nodes;
};

#endif
