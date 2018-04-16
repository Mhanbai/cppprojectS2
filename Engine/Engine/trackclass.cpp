#include "trackclass.h"

TrackClass::TrackClass()
{
	m_Terrain = 0;
}

TrackClass::TrackClass(const TrackClass &)
{
}

TrackClass::~TrackClass()
{
}

bool TrackClass::InitializeTrack(ID3D11Device* device, TerrainClass* terrain_in, int terrainWidth, int terrainHeight, WCHAR* textureFilename)
{
	m_Terrain = terrain_in;

	int index;
	int BOX_SIZE = 16;
	int mapNodeWidth = (terrainWidth / BOX_SIZE);
	int nodeCount = 0;

	//Create Graph///////////////////////////////////////////////////////////////////////////////
	//Check each 16x16 square in the heightmap
	for (int j = 0; j < terrainHeight; j+=BOX_SIZE)
	{
		for (int i = 0; i < terrainWidth; i+=BOX_SIZE)
		{
			float lowest = 0.0f;
			float highest = 0.0f;

			//If any point in the 8x8 square is over zero, square is not flat
			bool flat = true;
			for (int h = j; h < (j + BOX_SIZE); h++) {
				for (int w = i; w < (i + BOX_SIZE); w++) {
					index = (terrainHeight * h) + w;
					if (m_Terrain->GetHeightMap()[index].y > highest) {
						highest = m_Terrain->GetHeightMap()[index].y;
					}
					if (m_Terrain->GetHeightMap()[index].y < lowest) {
						lowest = m_Terrain->GetHeightMap()[index].y;
					}
				}
			}

			float diff = highest - lowest;
			if (diff > 1.0f) {
				flat = false;
			}

			//Create a new node
			MapNodeType node;
			//Define boundaries
			node.centerPoint = (D3DXVECTOR3((float)i, 0.0f, (float)j) + D3DXVECTOR3((float)i + (float)BOX_SIZE, 0.0f, (float)j + (float)BOX_SIZE)) * 0.5f;
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

			node.distanceEstimate = 9999999999.0f;
			node.parent = -1;

			//Add to node list
			nodes.push_back(node);

			//Increment node count
			nodeCount++;
		}
	}

	//Seed random with time
	//srand(time(NULL));

	//Find Path///////////////////////////////////////////////////////////////////////////////
	while (nodesOnPath.size() < 60) {
		nodesOnPath.clear();
		closedList.clear();
		openList.clear();
		for (MapNodeType node : nodes) {
			node.parent = -1;
			node.distanceEstimate = 99999999999.9f;
		}

		int startNode = rand() % nodes.size();
		while (nodes[startNode].isFlat == false) {
			startNode = rand() % nodes.size();
		}

		nodes[startNode].distanceEstimate = 0;

		//Add starting node to visited list
		closedList.push_back(startNode);

		endNode = rand() % nodes.size();
		while (nodes[endNode].isFlat == false) {
			endNode = rand() % nodes.size();
		}

		while (ExplorePath(startNode) == false) {
			closedList.clear();
			openList.clear();
			closedList.push_back(startNode);
			endNode = rand() % nodes.size();
			while (nodes[endNode].isFlat == false) {
				endNode = rand() % nodes.size();
			}
		}

		BuildPath(endNode);
	}

	//Find catmul rom spline between points to use for the track///////////////////////////////////////////////
	for (int i = 0; i < nodesOnPath.size() - 3; i++) {
			D3DXVECTOR3 pointToAdd;
			D3DXVECTOR3 point1 = nodesOnPath[i];
			D3DXVECTOR3 point2 = nodesOnPath[i + 1];
			D3DXVECTOR3 point3 = nodesOnPath[i + 2];
			D3DXVECTOR3 point4 = nodesOnPath[i + 3];

			for (float inc = 0.0f; inc < 1.0f; inc += 0.25f) {
				D3DXVec3CatmullRom(&pointToAdd, &point1, &point2, &point3, &point4, inc);
				trackPoints.push_back(pointToAdd);
			}
	}

	//Calculate length of track to use for texturing
	D3DXVECTOR3 nodeSpaceVector = trackPoints[1] - trackPoints[0];
	nodeLength = D3DXVec3Length(&nodeSpaceVector);

	//Find indices for road model///////////////////////////////////////////////////////////////////////////////
	m_model = new GeometryType[trackPoints.size() * 4];
	D3DXVECTOR3 vectorToNextPoint;

	float ROAD_WIDTH = 4.0f;
	float ROAD_HEIGHT = 0.2f;

	for (int i = 0; i < trackPoints.size(); i++) {
		//Dont do this if we're on the last node
		if (i != (trackPoints.size() - 1)) {
			//Find forward vector for node
			vectorToNextPoint = trackPoints[i + 1] - trackPoints[i];
			D3DXVec3Normalize(&vectorToNextPoint, &vectorToNextPoint);
		}

		//Find left and right vectors for node
		D3DXVECTOR3 right = D3DXVECTOR3(vectorToNextPoint.z, vectorToNextPoint.y, -vectorToNextPoint.x);
		D3DXVECTOR3 left = -right;

		//Find starting position for player and starting direction
		if (i == 0) {
			playerStartPos = trackPoints[1] + left * 2.0f;
		}
		else if (i == 1) {
			carsStartDirection = vectorToNextPoint;
		}

		//Find racing line for opponent
		opponentRacingLine.push_back(trackPoints[i] + right * 2.0f);

		//Find indices for track
		D3DXVECTOR3 bottomRight = trackPoints[i] + (right * ROAD_WIDTH);
		D3DXVECTOR3 topRight = bottomRight + (left * 0.5f) + D3DXVECTOR3(0.0f, ROAD_HEIGHT, 0.0f);
		D3DXVECTOR3 bottomLeft = trackPoints[i] + (left * ROAD_WIDTH);
		D3DXVECTOR3 topLeft = bottomLeft + (right * 0.5f) + D3DXVECTOR3(0.0f, ROAD_HEIGHT, 0.0f);

		int m_Pos = i * 4;

		m_model[m_Pos].x = bottomLeft.x;
		m_model[m_Pos].y = bottomLeft.y;
		m_model[m_Pos].z = bottomLeft.z;

		m_model[m_Pos + 1].x = topLeft.x;
		m_model[m_Pos + 1].y = topLeft.y;
		m_model[m_Pos + 1].z = topLeft.z;

		m_model[m_Pos + 2].x = topRight.x;
		m_model[m_Pos + 2].y = topRight.y;
		m_model[m_Pos + 2].z = topRight.z;

		m_model[m_Pos + 3].x = bottomRight.x;
		m_model[m_Pos + 3].y = bottomRight.y;
		m_model[m_Pos + 3].z = bottomRight.z;
	}

	//Clean up the vectors used
	nodes.clear();
	nodes.shrink_to_fit();

	LoadTexture(device, textureFilename);
	InitializeBuffers(device);

	return true;
}

void TrackClass::Shutdown()
{
}

bool TrackClass::Render(ID3D11DeviceContext* deviceContext)
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

	return true;
}

int TrackClass::GetIndexCount()
{
	return index;
}

TextureClass* TrackClass::GetTexture()
{
	return m_Texture;
}

void TrackClass::DeleteVertices()
{
	// Release the vertex array 
	delete[] vertices;
	vertices = 0;
}

float TrackClass::DistanceToStart(int node)
{
	float totalDistance = 0.0f;
	int currentNode = node;

	while (currentNode != -1) {
		totalDistance += nodes[currentNode].distanceFromParent;
		currentNode = nodes[currentNode].parent;
	}

	return totalDistance;
}

bool TrackClass::ExplorePath(int currentNode)
{
	//For each node that neighbors the current node
	for (int i = 0; i < 8; i++) {
		//If it is not on the closed list
		if (std::find(closedList.begin(), closedList.end(), nodes[currentNode].neighbours[i]) == closedList.end()) {
			//If it is the goal
			if (nodes[currentNode].neighbours[i] == endNode) {
				//Update its parent and add it to the closed list
				nodes[nodes[currentNode].neighbours[i]].parent = currentNode;
				closedList.push_back(nodes[currentNode].neighbours[i]);
				break;
			}
			//If it is a valid node and is not already on the open list
			else if ((nodes[currentNode].neighbours[i] != -1) && (std::find(openList.begin(), openList.end(), nodes[currentNode].neighbours[i]) == openList.end())) {
				if (nodes[nodes[currentNode].neighbours[i]].isFlat) {
					//Its parent node becomes the current node
					nodes[nodes[currentNode].neighbours[i]].parent = currentNode;
					//Calculate its distance from the parent
					D3DXVECTOR3 vecFromParent = nodes[nodes[nodes[currentNode].neighbours[i]].parent].centerPoint - nodes[nodes[currentNode].neighbours[i]].centerPoint;
					nodes[nodes[currentNode].neighbours[i]].distanceFromParent = D3DXVec3Length(&vecFromParent);
					//Calculate its distance to the endGoal
					D3DXVECTOR3 vecToEndPoint = nodes[currentNode].centerPoint - nodes[nodes[currentNode].neighbours[i]].centerPoint;
					nodes[nodes[currentNode].neighbours[i]].distanceToGoal = D3DXVec3Length(&vecToEndPoint);
					//Calculate Distance Estimate
					nodes[nodes[currentNode].neighbours[i]].distanceEstimate = DistanceToStart(nodes[currentNode].neighbours[i]) + nodes[nodes[currentNode].neighbours[i]].distanceToGoal;
					//Add to the open list
					openList.push_back(nodes[currentNode].neighbours[i]);
				}
			}
			//If it is a valid node and IS on the open list
			else if (nodes[currentNode].neighbours[i] != -1) {
				//Store the nodes current parent
				int tempParent = nodes[nodes[currentNode].neighbours[i]].parent;
				//Make the nodes parent the current node
				nodes[nodes[currentNode].neighbours[i]].parent = currentNode;
				//Check if distance using the old parent is more than distance using the new parent
				if (nodes[nodes[currentNode].neighbours[i]].distanceEstimate > (DistanceToStart(nodes[currentNode].neighbours[i]) + nodes[nodes[currentNode].neighbours[i]].distanceToGoal)) {
					//If so, save the new distance score
					nodes[nodes[currentNode].neighbours[i]].distanceEstimate = DistanceToStart(nodes[currentNode].neighbours[i]) + nodes[nodes[currentNode].neighbours[i]].distanceToGoal;
				}
				else {
					//If not, reset back to the old parent
					nodes[nodes[currentNode].neighbours[i]].parent = tempParent;
				}
			}
		}
	}

	if (std::find(closedList.begin(), closedList.end(), endNode) == closedList.end()) {
		if (openList.size() > 0) {
			int nextNode = -1;
			for (int nodeOnList : openList) {
				if (nextNode == -1) {
					nextNode = nodeOnList;
				}
				else {
					if (nodes[nextNode].distanceEstimate > nodes[nodeOnList].distanceEstimate) {
						nextNode = nodeOnList;
					}
				}
			}

			openList.erase(std::find(openList.begin(), openList.end(), nextNode));
			closedList.push_back(nextNode);
			ExplorePath(nextNode);
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}

void TrackClass::BuildPath(int endNode)
{
	int currentNode = endNode;

	while (currentNode != -1) {
		nodesOnPath.push_back(nodes[currentNode].centerPoint);
		currentNode = nodes[currentNode].parent;
	}
}

D3DXVECTOR3 TrackClass::CalculateNormal(D3DXVECTOR3 triPoint1, D3DXVECTOR3 triPoint2, D3DXVECTOR3 triPoint3)
{
	D3DXVECTOR3 lineOne = triPoint2 - triPoint1;
	D3DXVECTOR3 lineTwo = triPoint3 - triPoint1;
	D3DXVECTOR3 normal;
	D3DXVec3Cross(&normal, &lineOne, &lineTwo);
	D3DXVec3Normalize(&normal, &normal);

	return normal;
}

bool TrackClass::InitializeBuffers(ID3D11Device* device)
{
	unsigned long* indices;
	int i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	float tu, tv;
	int m_indexCount;


	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = 24 * trackPoints.size();

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

	D3DXVECTOR3 normal;
	int vInc = 0;

	for (int i = 0; i < ((trackPoints.size() - 1) * 4); i+=4) {
		int BL1 = i;		//Bottom Left first point
		int BL2 = BL1 + 4;	//Bottom Left second point
		int TL1 = i + 1;	//Top Left first point
		int TL2 = TL1 + 4;	//Top Left second point
		int TR1 = i + 2;	//Top Right first point
		int TR2 = TR1 + 4;	//Top Right second point
		int BR1 = i + 3;	//Bottom Right first point
		int BR2 = BR1 + 4;	//Bottom Right second point

		//Left Side
		vertices[index].position = D3DXVECTOR3(m_model[BL1].x, m_model[BL1].y, m_model[BL1].z);
		vertices[index].texture = D3DXVECTOR2(0.0f, vInc * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BL2].x, m_model[BL2].y, m_model[BL2].z);
		vertices[index].texture = D3DXVECTOR2(0.0f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[TL1].x, m_model[TL1].y, m_model[TL1].z);
		vertices[index].texture = D3DXVECTOR2(0.075f, vInc * 0.25f);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		vertices[index].position = D3DXVECTOR3(m_model[TL1].x, m_model[TL1].y, m_model[TL1].z);
		vertices[index].texture = D3DXVECTOR2(0.075f, vInc * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BL2].x, m_model[BL2].y, m_model[BL2].z);
		vertices[index].texture = D3DXVECTOR2(0.0f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[TL2].x, m_model[TL2].y, m_model[TL2].z);
		vertices[index].texture = D3DXVECTOR2(0.075f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		//Top Side
		vertices[index].position = D3DXVECTOR3(m_model[TL2].x, m_model[TL2].y, m_model[TL2].z);
		vertices[index].texture = D3DXVECTOR2(0.075f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[TR2].x, m_model[TR2].y, m_model[TR2].z);
		vertices[index].texture = D3DXVECTOR2(0.925f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[TL1].x, m_model[TL1].y, m_model[TL1].z);
		vertices[index].texture = D3DXVECTOR2(0.075f, vInc * 0.25f);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		vertices[index].position = D3DXVECTOR3(m_model[TL1].x, m_model[TL1].y, m_model[TL1].z);
		vertices[index].texture = D3DXVECTOR2(0.075f, vInc * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[TR2].x, m_model[TR2].y, m_model[TR2].z);
		vertices[index].texture = D3DXVECTOR2(0.925f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[TR1].x, m_model[TR1].y, m_model[TR1].z);
		vertices[index].texture = D3DXVECTOR2(0.925f, vInc * 0.25f);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		//Right Side
		vertices[index].position = D3DXVECTOR3(m_model[TR1].x, m_model[TR1].y, m_model[TR1].z);
		vertices[index].texture = D3DXVECTOR2(0.925f, vInc * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BR2].x, m_model[BR2].y, m_model[BR2].z);
		vertices[index].texture = D3DXVECTOR2(1.0f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BR1].x, m_model[BR1].y, m_model[BR1].z);
		vertices[index].texture = D3DXVECTOR2(1.0f, vInc * 0.25f);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		vertices[index].position = D3DXVECTOR3(m_model[TR1].x, m_model[TR1].y, m_model[TR1].z);
		vertices[index].texture = D3DXVECTOR2(0.925f, vInc * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[TR2].x, m_model[TR2].y, m_model[TR2].z);
		vertices[index].texture = D3DXVECTOR2(0.925f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BR2].x, m_model[BR2].y, m_model[BR2].z);
		vertices[index].texture = D3DXVECTOR2(1.0f, (vInc + 1) * 0.25f);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		//Bottom Side
		vertices[index].position = D3DXVECTOR3(m_model[BL2].x, m_model[BL2].y, m_model[BL2].z);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BR2].x, m_model[BR2].y, m_model[BR2].z);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BL1].x, m_model[BL1].y, m_model[BL1].z);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		vertices[index].position = D3DXVECTOR3(m_model[BL1].x, m_model[BL1].y, m_model[BL1].z);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BR2].x, m_model[BR2].y, m_model[BR2].z);
		indices[index] = index;
		index++;

		vertices[index].position = D3DXVECTOR3(m_model[BR1].x, m_model[BR1].y, m_model[BR1].z);
		indices[index] = index;
		index++;

		normal = CalculateNormal(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
		vertices[index - 3].normal = normal;
		vertices[index - 2].normal = normal;
		vertices[index - 1].normal = normal;

		vInc++;
		if (vInc == 4) {
			vInc = 0;
		}
	}

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
	delete[] indices;
	indices = 0;

	return true;
}

bool TrackClass::LoadTexture(ID3D11Device* device, WCHAR* textureFilename)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the grass texture object.
	bool result = m_Texture->Initialize(device, textureFilename);
	if (!result)
	{
		return false;
	}
}
