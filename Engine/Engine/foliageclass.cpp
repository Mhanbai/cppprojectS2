///////////////////////////////////////////////////////////////////////////////
// Filename: foliageclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "foliageclass.h"


FoliageClass::FoliageClass()
{
	m_foliageArray = 0;
	m_Instances = 0;

	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_Texture = 0;
}


FoliageClass::FoliageClass(const FoliageClass& other)
{
}


FoliageClass::~FoliageClass()
{
}


bool FoliageClass::Initialize(ID3D11Device* device, WCHAR* textureFilename, TerrainClass* terrain_in, float minSlope_in, float maxSlope_in, 
							float minHeight_in, float maxHeight_in, float minScale_in, float maxScale_in, float heightScale_in, int frequency_in)
{
	bool result;

	m_Terrain = terrain_in;
	minSlope = minSlope_in;
	maxSlope = maxSlope_in;
	minHeight = minHeight_in;
	maxHeight = maxHeight_in;
	minScale = minScale_in;
	maxScale = maxScale_in;
	frequency = frequency_in;
	heightScale = heightScale_in;

	// Generate the positions of the foliage.
	result = GeneratePositions();
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and instance buffer that hold the geometry for the foliage model.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	// Set the initial wind rotation and direction.
	m_windRotation = 0.0f;
	m_windDirection = 1;

	return true;
}


void FoliageClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Release the vertex and instance buffers.
	ShutdownBuffers();

	// Release the foliage array.
	if(m_foliageArray)
	{
		delete [] m_foliageArray;
		m_foliageArray = 0;
	}

	return;
}


void FoliageClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and instance buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


bool FoliageClass::Frame(D3DXVECTOR3 cameraPosition, ID3D11DeviceContext* deviceContext)
{
	D3DXMATRIX rotateMatrix, translationMatrix, rotateMatrix2, finalMatrix, scalingMatrix;
	D3DXVECTOR3 modelPosition;
	int i;
	double angle;
	float rotation, windRotation;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	InstanceType* instancesPtr;


	// Update the wind rotation.
	if(m_windDirection == 1)
	{
		m_windRotation += 0.1f;
		if(m_windRotation > 10.0f)
		{
			m_windDirection = 2;
		}
	}
	else
	{
		m_windRotation -= 0.1f;
		if(m_windRotation < -10.0f)
		{
			m_windDirection = 1;
		}
	}

	// Load the instance buffer with the updated locations.
	for(i=0; i<m_foliageCount; i++)
	{
		// Get the position of this piece of foliage.
		modelPosition.x = m_foliageArray[i].x;
		modelPosition.y = m_foliageArray[i].y;
		modelPosition.z = m_foliageArray[i].z;

		// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
		angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

		// Convert rotation into radians.
		rotation = (float)angle * 0.0174532925f;

		// Setup the X rotation of the billboard.
		D3DXMatrixRotationY(&rotateMatrix, rotation);

		D3DXVECTOR3 scalingVector = D3DXVECTOR3(m_foliageArray[i].scale, m_foliageArray[i].scale * heightScale, 1.0f);
		D3DXQUATERNION rotationQuaternion;
		D3DXQuaternionRotationMatrix(&rotationQuaternion, &rotateMatrix);
		D3DXVECTOR3 translationVector = D3DXVECTOR3(modelPosition.x, modelPosition.y, modelPosition.z);

		D3DXMatrixTransformation(&m_Instances[i].matrix, NULL, NULL, &scalingVector, NULL, &rotationQuaternion, &translationVector);
	}

	// Lock the instance buffer so it can be written to.
	result = deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the instance buffer.
	instancesPtr = (InstanceType*)mappedResource.pData;

	// Copy the instances array into the instance buffer.
	memcpy(instancesPtr, (void*)m_Instances, (sizeof(InstanceType) * m_foliageCount));

	// Unlock the instance buffer.
	deviceContext->Unmap(m_instanceBuffer, 0);

	return true;
}


int FoliageClass::GetVertexCount()
{
	return m_vertexCount;
}


int FoliageClass::GetInstanceCount()
{
	return m_instanceCount;
}


ID3D11ShaderResourceView* FoliageClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool FoliageClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;
	int i;
	D3DXMATRIX matrix;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // Bottom left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top left.
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  // Bottom right.
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  // Bottom right.
	vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top left.
	vertices[4].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);  // Top right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 0.0f);

	// Set up the description of the vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the array now that the vertex buffer has been created and loaded.
	delete [] vertices;
	vertices = 0;

	// Set the number of instances in the array.
	m_instanceCount = m_foliageCount;

	// Create the instance array.
	m_Instances = new InstanceType[m_instanceCount];
	if(!m_Instances)
	{
		return false;
	}

	// Setup an initial matrix.
	D3DXMatrixIdentity(&matrix);

	// Load the instance array with data.
	for(i=0; i<m_instanceCount; i++)
	{
		m_Instances[i].matrix = matrix;
		m_Instances[i].color = D3DXVECTOR3(m_foliageArray[i].r, m_foliageArray[i].g, m_foliageArray[i].b);
	}

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = m_Instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void FoliageClass::ShutdownBuffers()
{
	// Release the instance buffer.
	if(m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	// Release the instance array.
	if(m_Instances)
	{
		delete [] m_Instances;
		m_Instances = 0;
	}

	return;
}


void FoliageClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	// Set the buffer strides.
	strides[0] = sizeof(VertexType); 
	strides[1] = sizeof(InstanceType); 

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;	
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex and instance buffers to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool FoliageClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void FoliageClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool FoliageClass::GeneratePositions()
{
	int i;
	float red, green;
	m_foliageCount = 0;

	for (int i = 0; i < m_Terrain->m_vertexCount; i += frequency) {
		float slope = 1.0f - m_Terrain->vertices[i].normal.y;
		float height = m_Terrain->vertices[i].position.y;
		if ((slope > minSlope) && (slope < maxSlope) && (height > minHeight) && (height < maxHeight)) {
			foliageLocations.push_back(m_Terrain->vertices[i].position);
			m_foliageCount++;
		}
	}

	// Create an array to store all the foliage information.
	m_foliageArray = new FoliageType[m_foliageCount];
	if(!m_foliageArray)
	{
		return false;
	}

	// Seed the random generator.
	srand((int)time(NULL));

	// Set random positions and random colors for each piece of foliage.
	for(i=0; i<m_foliageCount; i++)
	{
		m_foliageArray[i].x = foliageLocations[i].x;
		m_foliageArray[i].y = foliageLocations[i].y;
		m_foliageArray[i].z = foliageLocations[i].z;

		red = ((float)rand() / (float)(RAND_MAX)) * 1.0f;
		green = ((float)rand() / (float)(RAND_MAX)) * 1.0f;

		m_foliageArray[i].r = 1.0f;
		m_foliageArray[i].g = 1.0f;
		m_foliageArray[i].b = 1.0f;

		m_foliageArray[i].scale = minScale + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxScale - minScale)));
	}

	return true;
}