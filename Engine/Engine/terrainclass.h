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
#include "simplexnoisegenerator.h"

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

/////////////
// GLOBALS //
/////////////
const int TEXTURE_REPEAT = 1;


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	    D3DXVECTOR3 normal;
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool InitializeTerrain(ID3D11Device*, SimplexNoiseGenerator* m_noiseGenerator, int terrainWidth, int terrainHeight, float zoom, float scale, WCHAR* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int  GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

	D3DXVECTOR3 GetVertexPosition(int index);
	int GetVertexCount();

private:
	bool CalculateNormals();
	void ShutdownHeightMap();

	void CalculateTextureCoordinates();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	bool m_terrainGeneratedToggle;
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
	VertexType* vertices;
	TextureClass* m_Texture;
};

#endif