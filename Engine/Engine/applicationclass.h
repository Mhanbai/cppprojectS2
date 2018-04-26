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
const float SCREEN_DEPTH = 500.0f;
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
#include "skyplaneclass.h"
#include "skyplaneshaderclass.h"
#include "trackclass.h"
#include "modelshaderclass.h"
#include "lightshaderclass.h"
#include "modelclass.h"
#include "car.h"
#include "screenobjectclass.h"
#include "textureshaderclass.h"
#include "rendertextureclass.h"
#include "collisionmap.h"
#include "motionblurshaderclass.h"
#include "orthowindowclass.h"
#include "foliageclass.h"
#include "foliageshaderclass.h"
#include "depthshaderclass.h"
#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"


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
	bool RenderToRearViewTexture();
	bool RenderScene(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	bool RenderSceneDepth(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	bool RenderSceneToTexture();
	bool RenderDepthToTexture();
	bool RenderMotionBlurToTexture();
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();
	bool Render2DTextureScene();
	bool StartGame();

private:
	HWND m_hwnd;
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
	SkyPlaneClass *m_SkyPlane;
	SkyPlaneShaderClass* m_SkyPlaneShader;
	TrackClass* m_Racetrack;
	ModelShaderClass* m_ModelShader;
	LightShaderClass* m_LightShader;
	Car* m_PlayerCar;
	ModelClass* m_PlayerCarModel;
	Car* m_AICar;
	ModelClass* m_AICarModel;
	ScreenObjectClass *m_WingMirror, *m_TextBackdrop, *m_Winner, *m_Loser, *m_1, *m_2, *m_3;
	TextureShaderClass* m_TextureShader;
	D3DXMATRIX screenViewMatrix = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.1f, 1.0f);
	RenderTextureClass* m_RearViewTexture;
	ScreenObjectClass* m_RearView;
	CollisionMap* m_Collision;
	MotionBlurShaderClass* m_MotionBlurShader;
	RenderTextureClass *m_RenderTexture, *m_MotionBlurTexture, *m_DepthTexture, *m_DownSampleTexure, *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;
	OrthoWindowClass *m_SmallWindow, *m_FullScreenWindow;
	FoliageClass* m_BushFoliage;
	FoliageShaderClass* m_FoliageShader;
	D3DXMATRIX currViewMatrix, currProjMatrix, prevViewMatrix, prevProjMatrix;
	DepthShaderClass* m_DepthShader;
	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	VerticalBlurShaderClass* m_VerticalBlurShader;
	D3DXMATRIX identity;

	//Variable to save car position in case of collision
	D3DXVECTOR3 playerCarPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int gameState = 0;

	int terrainWidth = 1024;
	int terrainHeight = 1024;

	int m_screenWidth;
	int m_screenHeight;

	bool showTrack = true;
	bool countdownDone = false;
	float countdownTimer = 0.0f;

	ModelClass* m_flags;
	int checkPointFlags;
	int playerCurrentCheckPoint = 0;
	int aiCurrentCheckPoint = 0;
	int prevPlayerCurrentCheckPoint = 0;
	int prevAICurrentCheckPoint = 0;

	float playerTimer = 0.0f;
	float playerCP1Timer = 0.0f;
	float playerCP2Timer = 0.0f;
	float playerCP3Timer = 0.0f;

	float aiTimer = 0.0f;
	float aiCP1Timer = 0.0f;
	float aiCP2Timer = 0.0f;
	float aiCP3Timer = 0.0f;

	bool raceOver = false;
};

#endif