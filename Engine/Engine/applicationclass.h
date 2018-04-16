////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "terrainshaderclass.h"
#include "lightclass.h"
#include "simplexnoisegenerator.h"
#include "skydomeclass.h"
#include "skydomeshaderclass.h"
#include "trackclass.h"
#include "modelshaderclass.h"
#include "lightshaderclass.h"
#include "modelclass.h"
#include "car.h"
#include "screenobjectclass.h"
#include "textureshaderclass.h"
#include "rendertextureclass.h"
#include "collisionmap.h"
#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"
#include "orthowindowclass.h"
#include "foliageclass.h"
#include "foliageshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();
	bool RenderToTexture();
	bool RenderScene(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	bool RenderSceneToTexture();
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();
	bool Render2DTextureScene();

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	TerrainShaderClass* m_TerrainShader;
	LightClass* m_Light;
	SimplexNoiseGenerator m_NoiseGenerator;
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	TrackClass* m_Racetrack;
	ModelShaderClass* m_ModelShader;
	LightShaderClass* m_LightShader;
	Car* m_PlayerCar;
	ModelClass* m_PlayerCarModel;
	Car* m_AICar;
	ModelClass* m_AICarModel;
	ScreenObjectClass* m_WingMirror;
	TextureShaderClass* m_TextureShader;
	D3DXMATRIX screenViewMatrix = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.1f, 1.0f);
	RenderTextureClass* m_RearViewTexture;
	ScreenObjectClass* m_RearView;
	CollisionMap* m_Collision;
	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	VerticalBlurShaderClass* m_VerticalBlurShader;
	RenderTextureClass *m_RenderTexture, *m_DownSampleTexure, *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;
	OrthoWindowClass *m_SmallWindow, *m_FullScreenWindow;
	FoliageClass* m_BushFoliage;
	FoliageClass* m_TreeFoliage;
	FoliageShaderClass* m_FoliageShader;

	//Variable to save car position in case of collision
	D3DXVECTOR3 playerCarPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	int debugCount = 2;
	ModelClass* m_Model[2];

	int m_screenWidth;
	int m_screenHeight;
};

#endif