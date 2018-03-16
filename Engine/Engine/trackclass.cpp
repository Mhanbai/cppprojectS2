#include "trackclass.h"

TrackClass::TrackClass()
{
}

TrackClass::TrackClass(const TrackClass &)
{
}

TrackClass::~TrackClass()
{
}

bool TrackClass::InitializeTrack(GeometryType * heightMap, int terrainWidth, int terrainHeight)
{
	int index;
	int BOX_SIZE = 8;
	int mapNodeWidth = (terrainWidth / BOX_SIZE);
	int nodeCount = 0;

	//Check each 8x8 square in the heightmap
	for (int j = 0; j < terrainHeight; j+=BOX_SIZE)
	{
		for (int i = 0; i < terrainWidth; i+=BOX_SIZE)
		{
			//If any point in the 8x8 square is over zero, square is not flat
			bool flat = true;
			for (int h = j; h < (j + BOX_SIZE); h++) {
				for (int w = i; w < (i + BOX_SIZE); w++) {
					index = (terrainHeight * h) + w;
					if (heightMap[index].y > 0.0f) {
						flat = false;
					}
				}
			}

			//Create a new node
			MapNodeType node;
			//Define boundaries
			node.bottomLeft = D3DXVECTOR3((float)i, 0.0f, (float)j);
			node.topRight = D3DXVECTOR3((float)i + (float)BOX_SIZE, 0.0f, (float)j + (float)BOX_SIZE);
			//Is node flat?
			node.isFlat = flat;
			//Assign index
			node.index = nodeCount;
			//Assign neighboring indexes
			node.neighbours[0] = nodeCount - mapNodeWidth - 1; //BottomLeft
			node.neighbours[1] = nodeCount - 1; //Left
			node.neighbours[2] = nodeCount + mapNodeWidth - 1; //TopLeft
			node.neighbours[3] = nodeCount + mapNodeWidth; //Top
			node.neighbours[4] = nodeCount + mapNodeWidth + 1; //TopRight
			node.neighbours[5] = nodeCount + 1; //Right
			node.neighbours[6] = nodeCount - mapNodeWidth + 1; //BottomRight
			node.neighbours[7] = nodeCount - mapNodeWidth; //Bottom
			//If neighbour does not exist set to -1
			if (nodeCount < mapNodeWidth) {
				node.neighbours[0] = -1;
				node.neighbours[6] = -1;
				node.neighbours[7] = -1;
			}
			if (nodeCount > ((mapNodeWidth * mapNodeWidth) - mapNodeWidth))
			{
				node.neighbours[2] = -1;
				node.neighbours[3] = -1;
				node.neighbours[4] = -1;
			}
			if (nodeCount % mapNodeWidth == 0) {
				node.neighbours[0] = -1;
				node.neighbours[1] = -1;
				node.neighbours[2] = -1;
			}
			if (nodeCount % mapNodeWidth == (mapNodeWidth - 1)) {
				node.neighbours[4] = -1;
				node.neighbours[5] = -1;
				node.neighbours[6] = -1;
			}
			//Add to node list
			nodes.push_back(node);
			//Increment node count
			nodeCount++;
		}
	}

	//Seed random with time
	srand(time(NULL));
	//Variable to store how long the generated track is
	int trackLength = 0;

	//Track must be over 19 nodes long in order to be used
	while (trackLength < 20) {
		//Start in random position
		int startingNode = rand() % (nodes.size() + 1);
		//Call the create track function
		trackLength = CreateTrack(startingNode);
	}

	int validNodes = nodesOnPath.size() - (nodesOnPath.size() % 4);

	//If our list is a multiple of 4, find catmul rom spline between points to use for the track
	for (int i = 0; i < validNodes; i += 4) {
			D3DXVECTOR3 pointToAdd;
			D3DXVECTOR3 point1 = nodes[nodesOnPath[i]].GetCenterPoint();
			D3DXVECTOR3 point2 = nodes[nodesOnPath[i + 1]].GetCenterPoint();
			D3DXVECTOR3 point3 = nodes[nodesOnPath[i + 2]].GetCenterPoint();
			D3DXVECTOR3 point4 = nodes[nodesOnPath[i + 3]].GetCenterPoint();

			for (float inc = 0.0f; inc < 1.0f; inc += 0.2f) {
				D3DXVec3CatmullRom(&pointToAdd, &point1, &point2, &point3, &point4, inc);
				trackPoints.push_back(pointToAdd);
			}
	}

	m_model = new GeometryType[trackPoints.size() * 4];
	D3DXVECTOR3 vectorToNextPoint;

	for (int i = 0; i < trackPoints.size(); i++) {
		if (i != (trackPoints.size() - 1)) {
			vectorToNextPoint = trackPoints[i + 1] - trackPoints[i];
			D3DXVec3Normalize(&vectorToNextPoint, &vectorToNextPoint);
		}

		D3DXVECTOR3 right = D3DXVECTOR3(vectorToNextPoint.z, vectorToNextPoint.y, -vectorToNextPoint.x);
		D3DXVECTOR3 left = -right;

		D3DXVECTOR3 bottomRight = trackPoints[i] + (right * 4.0f);
		D3DXVECTOR3 topRight = bottomRight + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		D3DXVECTOR3 bottomLeft = trackPoints[i] + (left * 4.0f);
		D3DXVECTOR3 topLeft = bottomLeft + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

		m_model[i].x = bottomLeft.x;
		m_model[i].y = bottomLeft.y;
		m_model[i].z = bottomLeft.z;

		m_model[i + 1].x = topLeft.x;
		m_model[i + 1].y = topLeft.y;
		m_model[i + 1].z = topLeft.z;

		m_model[i + 2].x = topRight.x;
		m_model[i + 2].y = topRight.y;
		m_model[i + 2].z = topRight.z;

		m_model[i + 3].x = bottomRight.x;
		m_model[i + 3].y = bottomRight.y;
		m_model[i + 3].z = bottomRight.z;
	}

	//Clean up the vectors used
	nodes.clear();
	nodes.shrink_to_fit();
	nodesOnPath.clear();
	nodesOnPath.shrink_to_fit();

	return true;
}

void TrackClass::Shutdown()
{
}

void TrackClass::Render(ID3D11DeviceContext *)
{
}

void TrackClass::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

int TrackClass::CreateTrack(int node)
{
	//Add current node to the node list
	nodesOnPath.push_back(node);

	//For the next neighboring node
	for (int i = 0; i < 8; i++) {
		//If it exists and is flat...
		if ((nodes[node].neighbours[i] != -1) && (nodes[nodes[node].neighbours[i]].isFlat)) {
			//...and has not already been used
			if (std::find(nodesOnPath.begin(), nodesOnPath.end(), nodes[node].neighbours[i]) == nodesOnPath.end()) {
				//Then move the path on to this node
				CreateTrack(nodes[node].neighbours[i]);
				//If there are no valid nodes left, return the size of the track
				return nodesOnPath.size();
			}
		}
	}

	//If there are no valid nodes left, return the size of the track
	return nodesOnPath.size();
}

bool TrackClass::InitializeBuffers(ID3D11Device* device)
{
	unsigned long* indices;
	VertexType* vertices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	float tu, tv;
	int m_vertexCount, m_indexCount;


	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = 12 + (12 * (trackPoints.size() - 1));

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize the index to the vertex buffer.
	index = 0;



	// Load the vertex and index array with the terrain data.
	/*index1 = (m_terrainHeight * j) + i;          // Bottom left.
	index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
	index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
	index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

	// Upper left.
	vertices[index].position = D3DXVECTOR3(m_model[index3].x, m_model[index3].y, m_model[index3].z);
	indices[index] = index;
	index++;

	// Upper right.
	vertices[index].position = D3DXVECTOR3(m_model[index4].x, m_model[index4].y, m_model[index4].z);
	indices[index] = index;
	index++;

	// Bottom left.
	vertices[index].position = D3DXVECTOR3(m_model[index1].x, m_model[index1].y, m_model[index1].z);
	indices[index] = index;
	index++;

	// Bottom left.
	vertices[index].position = D3DXVECTOR3(m_model[index1].x, m_model[index1].y, m_model[index1].z);
	indices[index] = index;
	index++;

	// Upper right.
	vertices[index].position = D3DXVECTOR3(m_model[index4].x, m_model[index4].y, m_model[index4].z);
	indices[index] = index;
	index++;

	vertices[index].position = D3DXVECTOR3(m_model[index2].x, m_model[index2].y, m_model[index2].z);
	indices[index] = index;
	index++;*/

	// Set up the description of the static vertex buffer.
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

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the index array now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	// Release the index array now that the buffers have been created and loaded.
	delete[] indices;
	indices = 0;

	return true;
}
