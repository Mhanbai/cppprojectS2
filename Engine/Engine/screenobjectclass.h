////////////////////////////////////////////////////////////////////////////////
// Filename: screenobjectclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCREENOBJECTCLASS_H_
#define _SCREENOBJECTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ScreenObjectClass
////////////////////////////////////////////////////////////////////////////////
class ScreenObjectClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	ScreenObjectClass();
	ScreenObjectClass(const ScreenObjectClass&);
	~ScreenObjectClass();

	bool Initialize(ID3D11Device*, int, int, WCHAR*, int, int);
	bool Initialize(ID3D11Device*, int, int, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, bool);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	int GetWidth();
	int GetHeight();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int, bool);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	int m_screenWidth, m_screenHeight;
	int m_screenobjectWidth, m_screenobjectHeight;
	int m_previousPosX, m_previousPosY;
	int m_width, m_height;
};

#endif