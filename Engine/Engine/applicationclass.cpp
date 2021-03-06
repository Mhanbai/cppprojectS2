////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_Light = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_SkyPlane = 0;
	m_SkyPlaneShader = 0;
	m_Racetrack = 0;
	m_ModelShader = 0;
	m_LightShader = 0;
	m_TextureShader = 0;
	m_WingMirror = 0;
	m_1 = 0;
	m_2 = 0;
	m_3 = 0;
	m_Winner = 0;
	m_Loser = 0;
	m_TextBackdrop = 0;
	m_RearViewTexture = 0;
	m_RearView = 0;
	m_PlayerCar = 0;
	m_PlayerCarModel = 0;
	m_AICar = 0;
	m_AICarModel = 0;
	m_Collision = 0;
	m_MotionBlurShader = 0;
	m_RenderTexture = 0;
	m_MotionBlurTexture = 0;
	m_FullScreenWindow = 0;
	m_BushFoliage = 0;
	m_FoliageShader = 0;
	m_DepthShader = 0;
	m_DepthTexture = 0;
	m_HorizontalBlurShader = 0;
	m_VerticalBlurShader = 0;
	m_DownSampleTexure = 0;
	m_HorizontalBlurTexture = 0;
	m_VerticalBlurTexture = 0;
	m_UpSampleTexure = 0;
	m_SmallWindow = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	int downSampleWidth, downSampleHeight;
	m_hwnd = hwnd;

	// Set the size to sample down to.
	downSampleWidth = screenWidth / 2;
	downSampleHeight = screenHeight / 2;

	char videoCard[128];
	int videoMemory;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Create an identity matrix
	D3DXMatrixIdentity(&identity);

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the model shader object.
	m_ModelShader = new ModelShaderClass;
	if (!m_ModelShader)
	{
		return false;
	}

	// Initialize the model shader object.
	result = m_ModelShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, screenViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f,-1.0f, 0.0f);

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane object.
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// Initialize the sky plane object.
	result = m_SkyPlane->Initialize(m_Direct3D->GetDevice(), L"data/cloud001.dds", L"data/cloud002.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane shader object.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// Initialize the sky plane shader object.
	result = m_SkyPlaneShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane shader object.", L"Error", MB_OK);
		return false;
	}



	// Create the bitmap object.
	m_Winner = new ScreenObjectClass;
	if (!m_Winner)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Winner->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/winner.dds", 495, 185);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_1 = new ScreenObjectClass;
	if (!m_1)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_1->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/1.dds", 89, 135);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_2 = new ScreenObjectClass;
	if (!m_1)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_2->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/2.dds", 126, 136);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_3 = new ScreenObjectClass;
	if (!m_3)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_3->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/3.dds", 122, 137);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	m_WingMirror = new ScreenObjectClass;
	if (!m_WingMirror)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_WingMirror->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/wingmirror.dds", int(screenWidth / 2.5f), int(screenHeight / 5.6f));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_TextBackdrop = new ScreenObjectClass;
	if (!m_TextBackdrop)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_TextBackdrop->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/textbackdrop.dds", 100, 400);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Winner = new ScreenObjectClass;
	if (!m_Winner)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Winner->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/winner.dds", 495, 185);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	m_Loser = new ScreenObjectClass;
	if (!m_Loser)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Loser->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"data/youlose.dds", 495, 185);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	// Create the render to texture object.
	m_RearViewTexture = new RenderTextureClass;
	if (!m_RearViewTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RearViewTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		return false;
	}

	// Create the debug window object.////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_RearView = new ScreenObjectClass;
	if (!m_RearView)
	{
		return false;
	}

	// Initialize the debug window object.////////////////////////////////////////////////////////////////////////////////////////////////////
	result = m_RearView->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, int(screenWidth / 2.67f), int(screenHeight / 6.0f));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

	// Create the Motion blur shader object.
	m_MotionBlurShader = new MotionBlurShaderClass;
	if (!m_MotionBlurShader)
	{
		return false;
	}

	// Initialize the Motion blur shader object.
	result = m_MotionBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Motion blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_DepthTexture = new RenderTextureClass;
	if (!m_DepthTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_DepthTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the Motion blur render to texture object.
	m_MotionBlurTexture = new RenderTextureClass;
	if (!m_MotionBlurTexture)
	{
		return false;
	}

	// Initialize the Motion blur render to texture object.
	result = m_MotionBlurTexture->Initialize(m_Direct3D->GetDevice(), m_screenWidth, m_screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Motion blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the full screen ortho window object.
	m_FullScreenWindow = new OrthoWindowClass;
	if (!m_FullScreenWindow)
	{
		return false;
	}

	// Initialize the full screen ortho window object.
	result = m_FullScreenWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}

	m_FoliageShader = new FoliageShaderClass;
	if (!m_FoliageShader) {
		return false;
	}

	result = m_FoliageShader->Initialize(m_Direct3D->GetDevice(), hwnd);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	result = m_Terrain->InitializeTerrain(m_Direct3D->GetDevice(), &m_NoiseGenerator, terrainWidth, terrainHeight,
		L"data/grass.dds", L"data/slope.dds", L"data/rock.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	m_Racetrack = new TrackClass;
	if (!m_Racetrack)
	{
		return false;
	}

	showTrack = m_Racetrack->InitializeTrack(m_Direct3D->GetDevice(), m_Terrain, 1024, 1024, L"data/track.dds");

	m_Collision = new CollisionMap;
	if (!m_Collision) {
		return false;
	}

	// Create the car object. 
	m_PlayerCar = new Car;
	if (!m_PlayerCar)
	{
		return false;
	}
	
	// Create the car object. 
	m_AICar = new Car;
	if (!m_AICar)
	{
		return false;
	}

	// Create the bush foliage object.
	m_BushFoliage = new FoliageClass;
	if (!m_BushFoliage)
	{
		return false;
	}

	// Create the depth shader object.
	m_DepthShader = new DepthShaderClass;
	if (!m_DepthShader)
	{
		return false;
	}

	// Initialize the depth shader object.
	result = m_DepthShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur shader object.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	if (!m_HorizontalBlurShader)
	{
		return false;
	}

	// Initialize the horizontal blur shader object.
	result = m_HorizontalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur shader object.
	m_VerticalBlurShader = new VerticalBlurShaderClass;
	if (!m_VerticalBlurShader)
	{
		return false;
	}

	// Initialize the vertical blur shader object.
	result = m_VerticalBlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the down sample render to texture object.
	m_DownSampleTexure = new RenderTextureClass;
	if (!m_DownSampleTexure)
	{
		return false;
	}

	// Initialize the down sample render to texture object.
	result = m_DownSampleTexure->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur render to texture object.
	m_HorizontalBlurTexture = new RenderTextureClass;
	if (!m_HorizontalBlurTexture)
	{
		return false;
	}

	// Initialize the horizontal blur render to texture object.
	result = m_HorizontalBlurTexture->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur render to texture object.
	m_VerticalBlurTexture = new RenderTextureClass;
	if (!m_VerticalBlurTexture)
	{
		return false;
	}

	// Initialize the vertical blur render to texture object.
	result = m_VerticalBlurTexture->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the up sample render to texture object.
	m_UpSampleTexure = new RenderTextureClass;
	if (!m_UpSampleTexure)
	{
		return false;
	}

	// Initialize the up sample render to texture object.
	result = m_UpSampleTexure->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the small ortho window object.
	m_SmallWindow = new OrthoWindowClass;
	if (!m_SmallWindow)
	{
		return false;
	}

	// Initialize the small ortho window object.
	result = m_SmallWindow->Initialize(m_Direct3D->GetDevice(), downSampleWidth, downSampleHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the small ortho window object.
	if (m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}

	// Release the up sample render to texture object.
	if (m_UpSampleTexure)
	{
		m_UpSampleTexure->Shutdown();
		delete m_UpSampleTexure;
		m_UpSampleTexure = 0;
	}

	// Release the vertical blur render to texture object.
	if (m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->Shutdown();
		delete m_VerticalBlurTexture;
		m_VerticalBlurTexture = 0;
	}

	// Release the horizontal blur render to texture object.
	if (m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}

	// Release the down sample render to texture object.
	if (m_DownSampleTexure)
	{
		m_DownSampleTexure->Shutdown();
		delete m_DownSampleTexure;
		m_DownSampleTexure = 0;
	}

	// Release the vertical blur shader object.
	if (m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// Release the horizontal blur shader object.
	if (m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release the depth shader object.
	if (m_DepthShader)
	{
		m_DepthShader->Shutdown();
		delete m_DepthShader;
		m_DepthShader = 0;
	}

	// Release the foliage object.
	if (m_BushFoliage)
	{
		m_BushFoliage->Shutdown();
		delete m_BushFoliage;
		m_BushFoliage = 0;
	}

	if (m_AICar)
	{
		m_AICar->Shutdown();
		delete m_AICar;
		m_AICar = 0;
	}

	if (m_PlayerCar)
	{
		m_PlayerCar->Shutdown();
		delete m_PlayerCar;
		m_PlayerCar = 0;
	}

	if (m_Collision) {
		m_Collision->ShutDown();
		delete m_Collision;
		m_Collision = 0;
	}

	if (m_Racetrack) {
		m_Racetrack->Shutdown();
		delete m_Racetrack;
		m_Racetrack = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	if (m_FoliageShader)
	{
		m_FoliageShader->Shutdown();
		delete m_FoliageShader;
		m_FoliageShader = 0;
	}

	// Release the full screen ortho window object.
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// Release the Motion blur render to texture object.
	if (m_MotionBlurTexture)
	{
		m_MotionBlurTexture->Shutdown();
		delete m_MotionBlurTexture;
		m_MotionBlurTexture = 0;
	}

	// Release the Motion blur render to texture object.
	if (m_DepthTexture)
	{
		m_DepthTexture->Shutdown();
		delete m_DepthTexture;
		m_DepthTexture = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the Motion blur shader object.
	if (m_MotionBlurShader)
	{
		m_MotionBlurShader->Shutdown();
		delete m_MotionBlurShader;
		m_MotionBlurShader = 0;
	}

	// Release the rearview window object.
	if (m_RearView)
	{
		m_RearView->Shutdown();
		delete m_RearView;
		m_RearView = 0;
	}

	// Release the render to texture object.
	if (m_RearViewTexture)
	{
		m_RearViewTexture->Shutdown();
		delete m_RearViewTexture;
		m_RearViewTexture = 0;
	}

	// Release the bitmap object.
	if (m_Loser)
	{
		m_Loser->Shutdown();
		delete m_Loser;
		m_Loser = 0;
	}

	// Release the bitmap object.
	if (m_Winner)
	{
		m_Winner->Shutdown();
		delete m_Winner;
		m_Winner = 0;
	}


	// Release the bitmap object.
	if (m_TextBackdrop)
	{
		m_TextBackdrop->Shutdown();
		delete m_TextBackdrop;
		m_TextBackdrop = 0;
	}
	
	// Release the bitmap object.
	if (m_WingMirror)
	{
		m_WingMirror->Shutdown();
		delete m_WingMirror;
		m_WingMirror = 0;
	}

	// Release the rearview window object.
	if (m_3)
	{
		m_3->Shutdown();
		delete m_3;
		m_3 = 0;
	}

	// Release the rearview window object.
	if (m_2)
	{
		m_2->Shutdown();
		delete m_2;
		m_2 = 0;
	}

	// Release the rearview window object.
	if (m_1)
	{
		m_1->Shutdown();
		delete m_1;
		m_1 = 0;
	}

	// Release the sky plane shader object.
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// Release the sky plane object.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}

	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model shader object.
	if (m_ModelShader)
	{
		m_ModelShader->Shutdown();
		delete m_ModelShader;
		m_ModelShader = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;

	D3DXVECTOR3 cameraPosition;

	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime() / 1000);
	if (!result)
	{
		return false;
	}

	if (gameState == 0) {
		m_Direct3D->ChangeProjection(1.5f, 0.1f, 1200.0f);
		m_Camera->SetPosition(float(terrainWidth / 2.0f), 350.0f, float(terrainHeight / 2.0f));
		m_Camera->SetRotation(90.0f, 0.0f, 0.0f);
		m_Camera->RenderPreScene();
		cameraPosition = m_Camera->GetPosition();
	}
	else if ((gameState == 1) && (!countdownDone)) {
		//Set camera to follow player
		m_Camera->Follow(m_PlayerCar->GetPosition(), m_PlayerCar->GetForwardVector(), m_Timer->GetTime() / 1000);
		m_Camera->Render();

		countdownTimer += m_Timer->GetTime() / 1000;
		if (countdownTimer > 3.0f) {
			countdownDone = true;
		}
	}
	else if ((gameState == 1) && (countdownDone)) {
#		//Gameplay code///////////////////////////////////////////

		//Player Car Frame
		m_PlayerCar->Frame(m_Timer->GetTime() / 1000);
		//Check if car is colliding
		m_PlayerCar->SetColliding(m_Collision->CheckCollision(m_PlayerCar));
		//If last frame for player car resulted in a collision, reset to previous position
		if (m_PlayerCar->colliding) {
			m_PlayerCar->SetPosition(D3DXVECTOR3(playerCarPos.x, m_Collision->GetHeight(m_PlayerCar), playerCarPos.z));
		}
		//If not
		else {
			//Save position
			playerCarPos = m_PlayerCar->GetPosition();
			//Set car to correct position (clamped to either track height or zero
			m_PlayerCar->SetPosition(D3DXVECTOR3(playerCarPos.x, m_Collision->GetHeight(m_PlayerCar), playerCarPos.z));
		}

		//Frame for computer controlled car
		m_AICar->OpponentFrame(m_Timer->GetTime() / 1000);
		D3DXVECTOR3 aiCarPos = m_AICar->GetPosition();
		//Clamp to track height or zero
		m_AICar->SetPosition(D3DXVECTOR3(aiCarPos.x, m_Collision->GetHeight(m_AICar), aiCarPos.z));

		//Set camera to follow player
		m_Camera->Follow(playerCarPos, m_PlayerCar->GetForwardVector(), m_Timer->GetTime() / 1000);
		m_Camera->Render();

		//Update checkpoints
		playerCurrentCheckPoint = max(playerCurrentCheckPoint, m_Collision->CheckPoint(m_PlayerCar));
		aiCurrentCheckPoint = max(aiCurrentCheckPoint, m_Collision->CheckPoint(m_AICar));

		//If checkpoint for player is new...
		if (playerCurrentCheckPoint != prevPlayerCurrentCheckPoint) {
			if (playerCurrentCheckPoint == m_Racetrack->noOfCheckpoints) {
				gameState = 2;
			}
		}

		//If checkpoint for AI is new...
		if (aiCurrentCheckPoint != prevAICurrentCheckPoint) {
			if (aiCurrentCheckPoint == m_Racetrack->noOfCheckpoints) {
				gameState = 3;
			}
		}

		//Update previous checkpoint trackers
		prevPlayerCurrentCheckPoint = playerCurrentCheckPoint;
		prevAICurrentCheckPoint = aiCurrentCheckPoint;

		//////Info Display/////////////////////////////////////////
		float deltaTime = m_Timer->GetTime() / 1000;
		playerTimer += deltaTime;
		aiTimer += deltaTime;

		///Player///
		switch (playerCurrentCheckPoint) {
		case 0:
			playerCP1Timer = playerTimer;
			break;
		case 1:
			playerCP2Timer = playerTimer - playerCP1Timer;
			break;
		case 2:
			playerCP3Timer = playerTimer - playerCP1Timer - playerCP2Timer;
			break;
		default:
			break;
		}

		char infoBuffer[32];
		sprintf_s(infoBuffer, "Player");
		m_Text->UpdateSentence(m_Text->m_sentence3, infoBuffer, 10, 40, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());

		if (playerCP1Timer != 0.0f) {
			sprintf_s(infoBuffer, "CP1: %.0f:%.2f", floor((playerCP1Timer / 60)), fmod(playerCP1Timer, 60));
			m_Text->UpdateSentence(m_Text->m_sentence4, infoBuffer, 10, 60, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}
		else {
			sprintf_s(infoBuffer, "CP1: -:--:--");
			m_Text->UpdateSentence(m_Text->m_sentence4, infoBuffer, 10, 60, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}

		if (playerCP2Timer != 0.0f) {
			sprintf_s(infoBuffer, "CP2: %.0f:%.2f", floor((playerCP2Timer / 60)), fmod(playerCP2Timer, 60));
			m_Text->UpdateSentence(m_Text->m_sentence5, infoBuffer, 10, 80, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}
		else {
			sprintf_s(infoBuffer, "CP2: -:--:--");
			m_Text->UpdateSentence(m_Text->m_sentence5, infoBuffer, 10, 80, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}

		if (playerCP3Timer != 0.0f) {
			sprintf_s(infoBuffer, "CP3: %.0f:%.2f", floor((playerCP3Timer / 60)), fmod(playerCP3Timer, 60));
			m_Text->UpdateSentence(m_Text->m_sentence6, infoBuffer, 10, 100, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}
		else {
			sprintf_s(infoBuffer, "CP3: -:--:--");
			m_Text->UpdateSentence(m_Text->m_sentence6, infoBuffer, 10, 100, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}

		///Total///
		sprintf_s(infoBuffer, "Total: %.0f:%.2f", floor((aiTimer / 60)), fmod(aiTimer, 60));
		m_Text->UpdateSentence(m_Text->m_sentence7, infoBuffer, 10, 140, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());


		///AI///

		switch (aiCurrentCheckPoint) {
		case 0:
			aiCP1Timer = aiTimer;
			break;
		case 1:
			aiCP2Timer = aiTimer - aiCP1Timer;
			break;
		case 2:
			aiCP3Timer = aiTimer - aiCP1Timer - aiCP2Timer;
			break;
		default:
			break;
		}

		sprintf_s(infoBuffer, "Opponent");
		m_Text->UpdateSentence(m_Text->m_sentence8, infoBuffer, 10, 200, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());

		if (aiCP1Timer != 0.0f) {
			sprintf_s(infoBuffer, "CP1: %.0f:%.2f", floor((aiCP1Timer / 60)), fmod(aiCP1Timer, 60));
			m_Text->UpdateSentence(m_Text->m_sentence9, infoBuffer, 10, 220, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}
		else {
			sprintf_s(infoBuffer, "CP1: -:--:--");
			m_Text->UpdateSentence(m_Text->m_sentence9, infoBuffer, 10, 220, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}

		if (aiCP2Timer != 0.0f) {
			sprintf_s(infoBuffer, "CP2: %.0f:%.2f", floor((aiCP2Timer / 60)), fmod(aiCP2Timer, 60));
			m_Text->UpdateSentence(m_Text->m_sentence10, infoBuffer, 10, 240, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}
		else {
			sprintf_s(infoBuffer, "CP1: -:--:--");
			m_Text->UpdateSentence(m_Text->m_sentence10, infoBuffer, 10, 240, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}

		if (aiCP3Timer != 0.0f) {
			sprintf_s(infoBuffer, "CP3: %.0f:%.2f", floor((aiCP3Timer / 60)), fmod(aiCP3Timer, 60));
			m_Text->UpdateSentence(m_Text->m_sentence11, infoBuffer, 10, 260, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}
		else {
			sprintf_s(infoBuffer, "CP3: -:--:--");
			m_Text->UpdateSentence(m_Text->m_sentence11, infoBuffer, 10, 260, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
		}

		///Total///
		sprintf_s(infoBuffer, "Total: %.0f:%.2f", floor((aiTimer / 60)), fmod(aiTimer, 60));
		m_Text->UpdateSentence(m_Text->m_sentence12, infoBuffer, 10, 300, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());


		///Distance///
		sprintf_s(infoBuffer, "Distance: %.0fm", m_Racetrack->trackLength);
		m_Text->UpdateSentence(m_Text->m_sentence13, infoBuffer, 10, 360, 1.0f, 1.0f, 1.0f, m_Direct3D->GetDeviceContext());
	}
	else if (gameState == 2) {
		char info1Buffer[32];
		sprintf_s(info1Buffer, "Winner!");
		m_Text->UpdateSentence(m_Text->m_sentence2, info1Buffer, 10, 340, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());
	}
	else if (gameState == 3) {
		char info1Buffer[32];
		sprintf_s(info1Buffer, "Loser!");
		m_Text->UpdateSentence(m_Text->m_sentence2, info1Buffer, 10, 340, 1.0f, 0.0f, 0.0f, m_Direct3D->GetDeviceContext());
	}


	if (gameState >= 1) {
		// Do the frame processing for the foliage
		cameraPosition = m_Camera->GetPosition();

		result = m_BushFoliage->Frame(-cameraPosition, m_Direct3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		// Do the sky plane frame processing.
		m_SkyPlane->Frame();
	}

	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown;

	if ((gameState == 1) && (countdownDone)) {
		keyDown = false;

		keyDown = m_Input->IsLeftPressed();
		m_PlayerCar->TurnLeft(keyDown);

		keyDown = m_Input->IsRightPressed();
		m_PlayerCar->TurnRight(keyDown);

		keyDown = m_Input->IsUpPressed();
		m_PlayerCar->Accelerate(keyDown);

		keyDown = m_Input->IsDownPressed();
		m_PlayerCar->BreakReverse(keyDown);
	}
	else if (gameState == 0) {
		keyDown = false;

		keyDown = m_Input->IsAPressed();
		if (keyDown) {
			char info1Buffer[32];
			sprintf_s(info1Buffer, "Generating...");
			m_Text->UpdateSentence(m_Text->m_sentence3, info1Buffer, 10, 40, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

			RenderGraphics();

			m_Terrain->GenerateNewTerrain();

			sprintf_s(info1Buffer, "Searching...");
			m_Text->UpdateSentence(m_Text->m_sentence3, info1Buffer, 10, 40, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

			RenderGraphics();

			showTrack = m_Racetrack->GenerateTrack();
		}

		keyDown = m_Input->IsZPressed();
		if (keyDown) {
			char info1Buffer[32];
			sprintf_s(info1Buffer, "Searching...");
			m_Text->UpdateSentence(m_Text->m_sentence3, info1Buffer, 10, 40, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

			RenderGraphics();

 			showTrack = m_Racetrack->GenerateTrack();
		}

		char info1Buffer[32];

		if (!showTrack) {
			sprintf_s(info1Buffer, "No track. Retry");
		}
		else {
			sprintf_s(info1Buffer, "Track found!");
		}
		m_Text->UpdateSentence(m_Text->m_sentence3, info1Buffer, 10, 40, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

		char info2Buffer[32];
		if (!showTrack) {
			sprintf_s(info2Buffer, "Distance: 0m");
		}
		else {
			sprintf_s(info2Buffer, "Distance: %.0fm", m_Racetrack->trackLength);
		}
		m_Text->UpdateSentence(m_Text->m_sentence4, info2Buffer, 10, 60, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

		keyDown = m_Input->IsSpacePressed();

		if ((keyDown) && (showTrack)) {
			m_Direct3D->ChangeProjection(4.0f, 0.1f, 1000.0f);
			StartGame();
		}

		sprintf_s(info1Buffer, "A - New Terrain");
		m_Text->UpdateSentence(m_Text->m_sentence5, info1Buffer, 10, 100, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

		sprintf_s(info1Buffer, "Z - New Track");
		m_Text->UpdateSentence(m_Text->m_sentence6, info1Buffer, 10, 120, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

		if ((keyDown) && (!showTrack)) {
			sprintf_s(info1Buffer, "ERR: No track!");
			m_Text->UpdateSentence(m_Text->m_sentence7, info1Buffer, 10, 140, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());
		}
		else {
			sprintf_s(info1Buffer, "Space - Begin");
			m_Text->UpdateSentence(m_Text->m_sentence7, info1Buffer, 10, 140, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());
		}
	}

	D3DXVECTOR3 camPos = -m_Camera->GetPosition();

	return true;
}


bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;

	// Generate the view matrix based on the camera's position.
	if (gameState >= 1) {
		m_Camera->Render();

		// Render the rear view scene to the texture first.
		result = RenderToRearViewTexture();
		if (!result)
		{
			return false;
		}
	}

	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// First render the scene to a render texture.
	result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

	//If the race has started, use the motion blur post-processing3
	if (gameState == 1) {
		// Perform a Motion blur on the down sampled render texture.
		result = RenderDepthToTexture();
		if (!result)
		{
			return false;
		}

		// Perform a Motion blur on the down sampled render texture.
		result = RenderMotionBlurToTexture();
		if (!result)
		{
			return false;
		}
	}

	//If the race has finished, used the full screen blur post processing
	if (gameState > 1) {
		// Next down sample the render texture to a smaller sized texture.
		result = DownSampleTexture();
		if (!result)
		{
			return false;
		}

		// Perform a horizontal blur on the down sampled render texture.
		result = RenderHorizontalBlurToTexture();
		if (!result)
		{
			return false;
		}

		// Now perform a vertical blur on the horizontal blur render texture.
		result = RenderVerticalBlurToTexture();
		if (!result)
		{
			return false;
		}

		// Up sample the final blurred render texture to screen size again.
		result = UpSampleTexture();
		if (!result)
		{
			return false;
		}
	}

	// Render the blurred up sampled render texture to the screen.
	result = Render2DTextureScene();
	if (!result)
	{
		return false;
	}

	///////////////////////////////////2D UI RENDERING/////////////////////////////////////////////////////////////////

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Get the world, and ortho matrices for 2D rendering
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	if (gameState == 1) {
		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_WingMirror->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_WingMirror->GetWidth() / 2), 20, false);
		if (!result)
		{
			return false;
		}

		// Render the wingmirror with the texture shader.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_WingMirror->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, m_WingMirror->GetTexture());
		if (!result)
		{
			return false;
		}

		result = m_RearView->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_RearView->GetWidth() / 2), 25, true);
		if (!result)
		{
			return false;
		}

		// Render the debug window using the texture shader.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_RearView->GetIndexCount(), worldMatrix,
			screenViewMatrix, orthoMatrix, m_RearViewTexture->GetShaderResourceView());
		if (!result)
		{
			return false;
		}

		if (!countdownDone) {
			if ((countdownTimer >= 0.0f) && (countdownTimer < 1.0f)) {
				result = m_3->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_3->GetWidth() / 2), (m_screenHeight / 2) - (m_3->GetHeight() / 2), false);
				if (!result)
				{
					return false;
				}

				// Render the wingmirror with the texture shader.
				result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_3->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, m_3->GetTexture());
				if (!result)
				{
					return false;
				}
			}
			else if ((countdownTimer >= 1.0f) && (countdownTimer < 2.0f)) {
				result = m_2->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_2->GetWidth() / 2), (m_screenHeight / 2) - (m_2->GetHeight() / 2), false);
				if (!result)
				{
					return false;
				}

				// Render the wingmirror with the texture shader.
				result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_2->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, m_2->GetTexture());
				if (!result)
				{
					return false;
				}
			}
			else if ((countdownTimer >= 2.0f) && (countdownTimer < 3.0f)) {
				result = m_1->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_1->GetWidth() / 2), (m_screenHeight / 2) - (m_1->GetHeight() / 2), false);
				if (!result)
				{
					return false;
				}

				// Render the wingmirror with the texture shader.
				result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_1->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, m_1->GetTexture());
				if (!result)
				{
					return false;
				}
			}
		}
	}
	else if (gameState == 2) {
		result = m_Winner->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_Winner->GetWidth() / 2), (m_screenHeight / 2) - (m_Winner->GetHeight() / 2), false);
		if (!result)
		{
			return false;
		}

		// Render the wingmirror with the texture shader.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Winner->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, m_Winner->GetTexture());
		if (!result)
		{
			return false;
		}
	}
	else if (gameState == 3) {
		result = m_Loser->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_Loser->GetWidth() / 2), (m_screenHeight / 2) - (m_Winner->GetHeight() / 2), false);
		if (!result)
		{
			return false;
		}

		// Render the wingmirror with the texture shader.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Loser->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, m_Loser->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	//Render the text backdrop
	result = m_TextBackdrop->Render(m_Direct3D->GetDeviceContext(), 0, 0, false);
	if (!result)
	{
		return false;
	}

	// Render the backdrop with the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_TextBackdrop->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix, m_TextBackdrop->GetTexture());
	if (!result)
	{
		return false;
	}

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderSceneToTexture()
{
	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(currViewMatrix);
	m_Direct3D->GetProjectionMatrix(currProjMatrix);

	RenderScene(currViewMatrix, currProjMatrix);

	m_Camera->GetViewMatrix(prevViewMatrix);
	m_Direct3D->GetProjectionMatrix(prevProjMatrix);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderDepthToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_DepthTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_DepthTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	RenderSceneDepth(currViewMatrix, currProjMatrix);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderMotionBlurToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeX;
	bool result;

	// Store the screen width in a float that will be used in the Motion blur shader.
	screenSizeX = (float)m_MotionBlurTexture->GetTextureWidth();

	// Set the render target to be the render to texture.
	m_MotionBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_MotionBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_MotionBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the Motion blur shader and the down sampled render to texture resource.
	result = m_MotionBlurShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
		m_RenderTexture->GetShaderResourceView(), m_DepthTexture->GetShaderResourceView(), currViewMatrix, currProjMatrix, prevViewMatrix, prevProjMatrix);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::Render2DTextureScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	if (gameState == 1) {
		// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
			m_MotionBlurTexture->GetShaderResourceView());
		if (!result)
		{
			return false;
		}
	}
	else if (gameState == 0) {
		// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
			m_RenderTexture->GetShaderResourceView());
		if (!result)
		{
			return false;
		}
	}
	else if (gameState > 1) {
		// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
			m_UpSampleTexure->GetShaderResourceView());
		if (!result)
		{
			return false;
		}
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	return true;
}

bool ApplicationClass::StartGame()
{
	float cameraX, cameraY, cameraZ;
	bool result;
	gameState = 1;

	// Set the initial position of the camera.
	cameraX = m_Racetrack->trackPoints[1].x;
	cameraY = m_Racetrack->trackPoints[1].y;
	cameraZ = m_Racetrack->trackPoints[1].z;

	m_Camera->SetPosition(-cameraX, -cameraY, -cameraZ);

	result = m_Collision->Initialize(m_Direct3D->GetDevice(), m_Terrain, m_Racetrack);
	if (!result)
	{
	MessageBox(m_hwnd, L"Could not initialize the collision object.", L"Error", MB_OK);
	return false;
	}

	// Initialize the input object.
	result = m_PlayerCar->Initialize("data/c_main.txt", L"data/cars.dds", m_PlayerCarModel, m_Direct3D->GetDevice(), m_Racetrack->carsStartDirection, m_Text, m_Direct3D->GetDeviceContext());
	if (!result)
	{
	MessageBox(m_hwnd, L"Could not initialize the car object.", L"Error", MB_OK);
	return false;
	}

	m_PlayerCar->SetPosition(m_Racetrack->playerStartPos + D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_PlayerCar->SetPosition(D3DXVECTOR3 (m_PlayerCar->GetPosition().x, m_Collision->GetHeight(m_PlayerCar), m_PlayerCar->GetPosition().z));

	// Initialize the input object.
	result = m_AICar->Initialize("data/c_main.txt", L"data/cars.dds", m_AICarModel, m_Direct3D->GetDevice(), m_Racetrack->carsStartDirection, m_Text, m_Direct3D->GetDeviceContext());
	if (!result)
	{
	MessageBox(m_hwnd, L"Could not initialize the car object.", L"Error", MB_OK);
	return false;
	}

	m_AICar->SetPosition(m_Racetrack->opponentRacingLine[1] + D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_AICar->SetPosition(D3DXVECTOR3(m_AICar->GetPosition().x, m_Collision->GetHeight(m_AICar), m_AICar->GetPosition().z));
	m_AICar->SetRacingLine(m_Racetrack->opponentRacingLine);

	// Initialize the foliage object.
	//															WCHAR* textureFilename,		   terrain_in,	minSlope_in, maxSlope_in,	minHeight_in,	maxHeight_in,	minScale_in,	maxScale_in,	heightScale_in, frequency_in)
	result = m_BushFoliage->Initialize(m_Direct3D->GetDevice(), L"data/foliage.dds", m_Terrain,	0.001f,		 0.02f,			-5.0f,			3.0f,			2.0f,			5.0f,			1.0f,			7);
	if (!result)
	{
	MessageBox(m_hwnd, L"Could not initialize the foliage object.", L"Error", MB_OK);
	return false;
	}

	m_Terrain->DeleteVertices();

	checkPointFlags = (m_Racetrack->noOfCheckpoints * 2) - 2;
	m_flags = new ModelClass[checkPointFlags + 1];
	D3DXVECTOR3 vectorBetween;

	for (int i = 0; i < checkPointFlags; i++) {
		//Initialize the flag model
		m_flags[i].Initialize(m_Direct3D->GetDevice(), "data/flag1.txt", L"data/flag.dds");
		//For each 2 flags...
		if ((i % 2) == 0) {
			//Find the vector between the two sides
			vectorBetween = D3DXVECTOR3(m_Racetrack->checkPoints[i / 2].bottomLeft - m_Racetrack->checkPoints[i / 2].bottomRight);
			//Find the correct angle for the flag and set to the correct transform
			float angleOffset = atan2(vectorBetween.x, vectorBetween.z) - atan2(1.0f, 0.0f);
			m_flags[i].Transform(m_Collision->checkPointFlags[i], angleOffset);
		}
		//Otherwise...
		else {
			//Find the correct angle for the flag and set to the correct transform
			float angleOffset = atan2(vectorBetween.x, vectorBetween.z) - atan2(-1.0f, 0.0f);
			m_flags[i].Transform(m_Collision->checkPointFlags[i], angleOffset);
		}
	}

	m_flags[checkPointFlags].Initialize(m_Direct3D->GetDevice(), "data/finish.txt", L"data/finish.dds");
	m_flags[checkPointFlags].Scale(0.2f);
	vectorBetween = D3DXVECTOR3(m_Racetrack->checkPoints[m_Racetrack->noOfCheckpoints - 1].bottomLeft - m_Racetrack->checkPoints[m_Racetrack->noOfCheckpoints - 1].bottomRight);
	float angleOffset = atan2(vectorBetween.x, vectorBetween.z);
	m_flags[checkPointFlags].Transform((m_Racetrack->checkPoints[m_Racetrack->noOfCheckpoints - 1].bottomLeft) - (vectorBetween * 0.5f), angleOffset);
}

bool ApplicationClass::RenderScene(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	D3DXMATRIX worldMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	worldMatrix = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-cameraPosition.x, -cameraPosition.y, -cameraPosition.z, 1.0f);

	// Turn off back face culling.
	m_Direct3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_Direct3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_Direct3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Enable additive blending so the clouds blend with the sky dome color.
	m_Direct3D->EnableAdditiveBlendState();

	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(m_Direct3D->GetDeviceContext());

	m_SkyPlaneShader->Render(m_Direct3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyPlane->GetCloudTexture1(), m_SkyPlane->GetCloudTexture2(), m_SkyPlane->GetTranslation(0), m_SkyPlane->GetTranslation(1),
		m_SkyPlane->GetTranslation(2), m_SkyPlane->GetTranslation(3), m_SkyPlane->GetBrightness());

	m_Direct3D->TurnOffAlphaBlending();

	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetGrassTexture(),
		m_Terrain->GetSlopeTexture(), m_Terrain->GetRockTexture());
	if (!result)
	{
		return false;
	}

	if (showTrack) {
		result = m_Racetrack->Render(m_Direct3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_ModelShader->Render(m_Direct3D->GetDeviceContext(), m_Racetrack->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Racetrack->GetTexture()->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}

	if (gameState >= 1) {

		m_PlayerCarModel->Render(m_Direct3D->GetDeviceContext());

		result = m_ModelShader->Render(m_Direct3D->GetDeviceContext(), m_PlayerCarModel->GetIndexCount(), m_PlayerCarModel->GetWorldMatrix(), viewMatrix, projectionMatrix,
			m_PlayerCarModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}

		m_AICarModel->Render(m_Direct3D->GetDeviceContext());

		result = m_ModelShader->Render(m_Direct3D->GetDeviceContext(), m_AICarModel->GetIndexCount(), m_AICarModel->GetWorldMatrix(), viewMatrix, projectionMatrix,
			m_AICarModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}

		for (int i = 0; i < checkPointFlags + 1; i++) {
			m_flags[i].Render(m_Direct3D->GetDeviceContext());

			result = m_ModelShader->Render(m_Direct3D->GetDeviceContext(), m_flags[i].GetIndexCount(), m_flags[i].GetWorldMatrix(), viewMatrix, projectionMatrix,
				m_flags[i].GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
			if (!result)
			{
				return false;
			}
		}

		// Turn on the alpha-to-coverage blending.
		m_Direct3D->EnableAlphaToCoverageBlending();

		// Render the foliage.
		m_BushFoliage->Render(m_Direct3D->GetDeviceContext());

		result = m_FoliageShader->Render(m_Direct3D->GetDeviceContext(), m_BushFoliage->GetVertexCount(), m_BushFoliage->GetInstanceCount(), viewMatrix, projectionMatrix, m_BushFoliage->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	// Turn off the alpha blending.
	m_Direct3D->TurnOffAlphaBlending();

	return true;
}

bool ApplicationClass::RenderSceneDepth(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	D3DXMATRIX worldMatrix;
	D3DXVECTOR3 cameraPosition;
	bool result;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = m_Racetrack->Render(m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_Racetrack->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_PlayerCarModel->Render(m_Direct3D->GetDeviceContext());

	result = m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlayerCarModel->GetIndexCount(), m_PlayerCarModel->GetWorldMatrix(), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_AICarModel->Render(m_Direct3D->GetDeviceContext());

	result = m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_AICarModel->GetIndexCount(), m_AICarModel->GetWorldMatrix(), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::RenderToRearViewTexture()
{
	bool result;
	D3DXMATRIX viewMatrix, projectionMatrix;

	m_Camera->GetReverseViewMatrix(viewMatrix);
	m_Direct3D->ChangeProjection(8.0f, 0.1f, 1200.0f, m_screenWidth / 2.67f, m_screenHeight / 6.0f);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->ChangeProjection(4.0f, 0.1f, 1200.0f);

	// Set the render target to be the render to texture.
	m_RearViewTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_RearViewTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = RenderScene(viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool ApplicationClass::DownSampleTexture()
{
	D3DXMATRIX orthoMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_DownSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_DownSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), identity, screenViewMatrix, orthoMatrix,
		m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderHorizontalBlurToTexture()
{
	D3DXMATRIX orthoMatrix;
	float screenSizeX;
	bool result;

	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)m_HorizontalBlurTexture->GetTextureWidth();

	// Set the render target to be the render to texture.
	m_HorizontalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_HorizontalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	result = m_HorizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), identity, screenViewMatrix, orthoMatrix,
		m_DownSampleTexure->GetShaderResourceView(), screenSizeX);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderVerticalBlurToTexture()
{
	D3DXMATRIX orthoMatrix;
	float screenSizeY;
	bool result;


	// Store the screen height in a float that will be used in the vertical blur shader.
	screenSizeY = (float)m_VerticalBlurTexture->GetTextureHeight();

	// Set the render target to be the render to texture.
	m_VerticalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_VerticalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	result = m_VerticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), identity, screenViewMatrix, orthoMatrix,
		m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::UpSampleTexture()
{
	D3DXMATRIX orthoMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_UpSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_UpSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), identity, screenViewMatrix, orthoMatrix,
		m_VerticalBlurTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}