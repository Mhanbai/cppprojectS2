////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();

	void Follow(D3DXVECTOR3 followTarget, D3DXVECTOR3 targetForwardVector, float deltaTime);
	void Render();
	void RenderPreScene();
	void GetViewMatrix(D3DXMATRIX&);
	void GetReverseViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 carPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float distance;
	float height;
	float springConstant;
	float dampConstant;
};

#endif