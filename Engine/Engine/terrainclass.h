////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>
#include <time.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "simplexnoisegenerator.h"

/////////////
// GLOBALS //
/////////////
const int TEXTURE_REPEAT = 32;


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
struct VectorType
{
	float x, y, z;
};

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

class TerrainClass
{
public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool InitializeTerrain(ID3D11Device*, SimplexNoiseGenerator* noiseGenerator, int terrainWidth, int terrainHeight, 
							WCHAR* grassTexture, WCHAR* slopeTexture, WCHAR* rockTexture);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int  GetIndexCount();
	void DeleteVertices();

	ID3D11ShaderResourceView* GetGrassTexture();
	ID3D11ShaderResourceView* GetSlopeTexture();
	ID3D11ShaderResourceView* GetRockTexture();

	GeometryType* GetHeightMap();
	VertexType* vertices;
	int m_vertexCount;

	bool GenerateNewTerrain();

private:
	bool CalculateNormals();
	void ShutdownHeightMap();

	void CalculateTextureCoordinates();
	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ReleaseTextures();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	ID3D11Device* m_Device; 
	SimplexNoiseGenerator* m_noiseGenerator;
	int m_terrainWidth, m_terrainHeight;
	int m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	GeometryType* m_heightMap;
	TextureClass *m_GrassTexture, *m_SlopeTexture, *m_RockTexture;
	bool terrainGenerated = false;
};

#endif