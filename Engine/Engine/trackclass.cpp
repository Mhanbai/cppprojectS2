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

bool TrackClass::InitializeTrack(HeightMapType * heightMap, int terrainWidth, int terrainHeight)
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
			node.topLeft = D3DXVECTOR3((float)i, 0.0f, (float)j + (float)BOX_SIZE);
			node.topRight = D3DXVECTOR3((float)i + (float)BOX_SIZE, 0.0f, (float)j + (float)BOX_SIZE);
			node.bottomRight = D3DXVECTOR3((float)i + (float)BOX_SIZE, 0.0f, (float)j);
			//Assign flat or not
			node.isFlat = flat;
			//Assign neighboring indexes
			node.neighbours[0] = nodeCount - mapNodeWidth - 1; //BottomLeft
			node.neighbours[1] = nodeCount - 1; //Left
			node.neighbours[2] = nodeCount + mapNodeWidth - 1; //TopLeft
			node.neighbours[3] = nodeCount + mapNodeWidth; //Top
			node.neighbours[4] = nodeCount + mapNodeWidth + 1; //TopRight
			node.neighbours[5] = nodeCount + 1; //Right
			node.neighbours[6] = nodeCount - mapNodeWidth + 1; //BottomRight
			node.neighbours[7] = nodeCount - mapNodeWidth; //Bottom
			//If neighbour does not exist, set to -1
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

	/*std::vector<std::vector<MapNodeType>> potentialPaths;
	for (int i = 0; i < nodes.size(); i++) {
		std::vector<MapNodeType> path;
		path.push_back(nodes[i]);

	}*/
	
	return true;
}

void TrackClass::Shutdown()
{
}

void TrackClass::Render(ID3D11DeviceContext *)
{
}
