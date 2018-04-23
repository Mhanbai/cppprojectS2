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
	m_Racetrack = 0;
	m_ModelShader = 0;
	m_LightShader = 0;
	m_TextureShader = 0;
	m_WingMirror = 0;
	m_RearViewTexture = 0;
	m_RearView = 0;
	m_PlayerCar = 0;
	m_PlayerCarModel = 0;
	m_AICar = 0;
	m_AICarModel = 0;
	m_Collision = 0;
	m_HorizontalBlurShader = 0;
	m_VerticalBlurShader = 0;
	m_RenderTexture = 0;
	m_DownSampleTexure = 0;
	m_HorizontalBlurTexture = 0;
	m_VerticalBlurTexture = 0;
	m_UpSampleTexure = 0;
	m_SmallWindow = 0;
	m_FullScreenWindow = 0;
	m_BushFoliage = 0;
	m_TreeFoliage = 0;
	m_FoliageShader = 0;
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

	float cameraX, cameraY, cameraZ;
	char videoCard[128];
	int videoMemory;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	// Set the size to sample down to.
	downSampleWidth = screenWidth / 2;
	downSampleHeight = screenHeight / 2;

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

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

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
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
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

	// Create the bitmap object.
	m_WingMirror = new ScreenObjectClass;
	if (!m_WingMirror)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_WingMirror->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/wingmirror.dds", screenWidth / 2.5f, screenHeight / 5.6f);
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
	result = m_RearView->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, screenWidth / 2.67f, screenHeight / 6.0f);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
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

	// Initialize the terrain object.
	result = m_Terrain->InitializeTerrain(m_Direct3D->GetDevice(), &m_NoiseGenerator, terrainWidth, terrainHeight,
		L"../Engine/data/grass.dds", L"../Engine/data/slope.dds", L"../Engine/data/rock.dds");
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

	showTrack = m_Racetrack->InitializeTrack(m_Direct3D->GetDevice(), m_Terrain, 1024, 1024, L"../Engine/data/track.dds");

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

	// Create the tree foliage object.
	m_TreeFoliage = new FoliageClass;
	if (!m_TreeFoliage)
	{
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	if (m_FoliageShader)
	{
		m_FoliageShader->Shutdown();
		delete m_FoliageShader;
		m_FoliageShader = 0;
	}

	// Release the foliage object.
	if (m_BushFoliage)
	{
		m_BushFoliage->Shutdown();
		delete m_BushFoliage;
		m_BushFoliage = 0;
	}

	// Release the foliage object.
	if (m_TreeFoliage)
	{
		m_TreeFoliage->Shutdown();
		delete m_TreeFoliage;
		m_TreeFoliage = 0;
	}

	// Release the full screen ortho window object.
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

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

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
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
	if (m_WingMirror)
	{
		m_WingMirror->Shutdown();
		delete m_WingMirror;
		m_WingMirror = 0;
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
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
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

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
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
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
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
		m_Direct3D->ChangeFieldofView(2.0f, 0.1f, 1200.0f);
		m_Camera->SetPosition(terrainWidth / 2, 550.0f, terrainHeight / 2);
		m_Camera->SetRotation(90.0f, 0.0f, 0.0f);
		m_Camera->RenderPreScene();
		cameraPosition = m_Camera->GetPosition();
	} else if (gameState == 1) {
		m_PlayerCar->Frame(m_Timer->GetTime() / 1000);
		m_PlayerCar->SetColliding(m_Collision->CheckCollision(m_PlayerCar));


		//If last frame for player car resulted in a collision, reset to previous position
		if (m_PlayerCar->colliding) {
			m_PlayerCar->SetPosition(playerCarPos);
		}
		else {
			playerCarPos = m_PlayerCar->GetPosition();
			m_PlayerCar->SetPosition(D3DXVECTOR3(playerCarPos.x, m_Collision->GetHeight(m_PlayerCar), playerCarPos.z));
		}

		//m_AICar->OpponentFrame(m_Timer->GetTime() / 1000);
		D3DXVECTOR3 aiCarPos = m_AICar->GetPosition();
		m_AICar->SetPosition(D3DXVECTOR3(aiCarPos.x, m_Collision->GetHeight(m_AICar), aiCarPos.z));

		m_Camera->Follow(playerCarPos, m_PlayerCar->GetForwardVector(), m_Timer->GetTime() / 1000);
		m_Camera->Render();

		cameraPosition = m_Camera->GetPosition();
		// Do the frame processing for the foliage.
		result = m_BushFoliage->Frame(-cameraPosition, m_Direct3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_TreeFoliage->Frame(-cameraPosition, m_Direct3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
	}

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
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
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	if (gameState == 1) {
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
			m_Text->UpdateSentence(m_Text->m_sentence9, info1Buffer, 10, 230, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

			RenderGraphics();

			m_Terrain->GenerateNewTerrain();

			sprintf_s(info1Buffer, "Searching...");
			m_Text->UpdateSentence(m_Text->m_sentence9, info1Buffer, 10, 230, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

			RenderGraphics();

			showTrack = m_Racetrack->GenerateTrack();
		}

		keyDown = m_Input->IsZPressed();
		if (keyDown) {
			char info1Buffer[32];
			sprintf_s(info1Buffer, "Searching...");
			m_Text->UpdateSentence(m_Text->m_sentence9, info1Buffer, 10, 230, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

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
		m_Text->UpdateSentence(m_Text->m_sentence9, info1Buffer, 10, 230, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

		char info2Buffer[32];
		if (!showTrack) {
			sprintf_s(info2Buffer, "Distance: 0m");
		}
		else {
			sprintf_s(info2Buffer, "Distance: %.0fm", m_Racetrack->trackLength);
		}
		m_Text->UpdateSentence(m_Text->m_sentence10, info2Buffer, 10, 250, 0.0f, 1.0f, 0.0f, m_Direct3D->GetDeviceContext());

		keyDown = m_Input->IsSpacePressed();
		if (keyDown) {
			m_Direct3D->ChangeFieldofView(4.0f, 0.1f, 1000.0f);
			StartGame();
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

	// Render the entire scene to the texture first.
	result = RenderToTexture();
	if (!result)
	{
		return false;
	}

	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// First render the scene to a render texture.
	result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

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

	int text = m_RearView->GetWidth();

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	if (gameState == 1) {
		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_WingMirror->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_WingMirror->GetWidth() / 2), 20);
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

		result = m_RearView->Render(m_Direct3D->GetDeviceContext(), (m_screenWidth / 2) - (m_RearView->GetWidth() / 2), 25);
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
	}

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
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
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	RenderScene(viewMatrix, projectionMatrix);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::DownSampleTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_DownSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_DownSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
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
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeX;
	bool result;

	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)m_HorizontalBlurTexture->GetTextureWidth();

	// Set the render target to be the render to texture.
	m_HorizontalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_HorizontalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	result = m_HorizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
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
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeY;
	bool result;


	// Store the screen height in a float that will be used in the vertical blur shader.
	screenSizeY = (float)m_VerticalBlurTexture->GetTextureHeight();

	// Set the render target to be the render to texture.
	m_VerticalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_VerticalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	result = m_VerticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
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
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	// Set the render target to be the render to texture.
	m_UpSampleTexure->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_UpSampleTexure->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
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

	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), worldMatrix, screenViewMatrix, orthoMatrix,
		//m_UpSampleTexure->GetShaderResourceView());
		m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
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

	result = m_Collision->Initialize(m_Terrain, m_Racetrack);
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

	// Initialize the input object.
	result = m_AICar->Initialize("data/c_main.txt", L"data/cars.dds", m_AICarModel, m_Direct3D->GetDevice(), m_Racetrack->carsStartDirection, m_Text, m_Direct3D->GetDeviceContext());
	if (!result)
	{
	MessageBox(m_hwnd, L"Could not initialize the car object.", L"Error", MB_OK);
	return false;
	}

	m_AICar->SetPosition(m_Racetrack->opponentRacingLine[1] + D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_AICar->SetRacingLine(m_Racetrack->opponentRacingLine);

	// Initialize the foliage object.
	result = m_BushFoliage->Initialize(m_Direct3D->GetDevice(), L"../Engine/data/foliage.dds", m_Terrain, 0.01f, 0.03f, -5.0f, 3.0f, 1.0f, 4.0f, 3);
	if (!result)
	{
	MessageBox(m_hwnd, L"Could not initialize the foliage object.", L"Error", MB_OK);
	return false;
	}

	// Initialize the foliage object.
	result = m_TreeFoliage->Initialize(m_Direct3D->GetDevice(), L"../Engine/data/tree.dds", m_Terrain, 0.02f, 0.04f, -5.0f, 2.0f, 10.0f, 15.0f, 32);
	if (!result)
	{
	MessageBox(m_hwnd, L"Could not initialize the foliage object.", L"Error", MB_OK);
	return false;
	}

	m_Terrain->DeleteVertices();
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

	if (gameState == 1) {

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

		// Turn on the alpha-to-coverage blending.
		m_Direct3D->EnableAlphaToCoverageBlending();

		// Render the foliage.
		m_BushFoliage->Render(m_Direct3D->GetDeviceContext());

		result = m_FoliageShader->Render(m_Direct3D->GetDeviceContext(), m_BushFoliage->GetVertexCount(), m_BushFoliage->GetInstanceCount(), viewMatrix, projectionMatrix, m_BushFoliage->GetTexture());
		if (!result)
		{
			return false;
		}

		// Render the foliage.
		m_TreeFoliage->Render(m_Direct3D->GetDeviceContext());

		result = m_FoliageShader->Render(m_Direct3D->GetDeviceContext(), m_TreeFoliage->GetVertexCount(), m_TreeFoliage->GetInstanceCount(), viewMatrix, projectionMatrix, m_TreeFoliage->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	// Turn off the alpha blending.
	m_Direct3D->TurnOffAlphaBlending();

	return true;
}

bool ApplicationClass::RenderToTexture()
{
	bool result;
	D3DXMATRIX viewMatrix, projectionMatrix;

	m_Camera->GetReverseViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

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