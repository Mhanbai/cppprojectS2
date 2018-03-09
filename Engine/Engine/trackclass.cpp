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

	for (int j = 0; j < terrainHeight; j+=4)
	{
		for (int i = 0; i < terrainWidth; i+=4)
		{
			bool flat = true;
			for (int h = j; h < (j + 4); h++) {
				for (int w = i; w < (i + 4); w++) {
					index = (terrainHeight * h) + w;
					if (heightMap[index].y > 0.0f) {
						flat = false;
					}
				}
			}

			MapNodeType node;
			node.topLeft = D3DXVECTOR3((float)j + 4.0f, 0.0f, (float)i);
			node.bottomLeft = D3DXVECTOR3((float)j, 0.0f, (float)i);
			node.topRight = D3DXVECTOR3((float)j + 4.0f, 0.0f, (float)i + 4.0f);
			node.bottomRight = D3DXVECTOR3((float)j, 0.0f, (float)i + 4.0f);
			node.isFlat = flat;
			nodes.push_back(node);
		}
	}

	return true;
}

void TrackClass::Shutdown()
{
}

void TrackClass::Render(ID3D11DeviceContext *)
{
}
