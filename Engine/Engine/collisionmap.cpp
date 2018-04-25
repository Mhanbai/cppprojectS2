#include "collisionmap.h"

CollisionMap::CollisionMap()
{
}

CollisionMap::CollisionMap(const CollisionMap &)
{
}

CollisionMap::~CollisionMap()
{
}

bool CollisionMap::Initialize(ID3D11Device* m_device, TerrainClass * terrain_in, TrackClass* track_in)
{
	m_Terrain = terrain_in;
	m_Racetrack = track_in;

	for (int i = 0; i < m_Racetrack->m_vertexCount; i+=3) {
		VertexType a = m_Racetrack->vertices[i];
		VertexType b = m_Racetrack->vertices[i + 1];
		VertexType c = m_Racetrack->vertices[i + 2];

		D3DXVECTOR3 point1 = D3DXVECTOR3(a.position.x, a.position.y, a.position.z);
		D3DXVECTOR3 point2 = D3DXVECTOR3(b.position.x, b.position.y, b.position.z);
		D3DXVECTOR3 point3 = D3DXVECTOR3(c.position.x, c.position.y, c.position.z);

		trackCheck.push_back(Triangle(point1, point2, point3));
	}

	m_Racetrack->DeleteVertices();

	////////Set up checkpoints////////

	noOfCheckpoints = m_Racetrack->noOfCheckpoints;
	checkPointFlags = new D3DXVECTOR3[noOfCheckpoints * 2];

	for (int i = 0; i < noOfCheckpoints; i++) {
		checkPointsCheck.push_back(Triangle(m_Racetrack->checkPoints[i].bottomLeft, m_Racetrack->checkPoints[i].topLeft, m_Racetrack->checkPoints[i].topRight));
		checkPointsCheck.push_back(Triangle(m_Racetrack->checkPoints[i].topRight, m_Racetrack->checkPoints[i].bottomLeft, m_Racetrack->checkPoints[i].bottomRight));
	}

	for (int i = 0; i < (noOfCheckpoints * 2); i+=2) {
		checkPointFlags[i] = m_Racetrack->checkPoints[i /2].bottomLeft;
		checkPointFlags[i + 1] = m_Racetrack->checkPoints[i / 2].bottomRight;
	}

	for (int i = 0; i < m_Terrain->m_vertexCount; i += 3) {
		VertexType a = m_Terrain->vertices[i];
		VertexType b = m_Terrain->vertices[i + 1];
		VertexType c = m_Terrain->vertices[i + 2];

		if (((a.position.y > 2.0f) || (b.position.y > 2.0f) || (c.position.y > 2.0f)) &&
			((a.position.y < 3.0f) || (b.position.y < 3.0f) || (c.position.y < 3.0f)))
		{
			D3DXVECTOR3 point1 = D3DXVECTOR3(a.position.x, a.position.y, a.position.z);
			D3DXVECTOR3 point2 = D3DXVECTOR3(b.position.x, b.position.y, b.position.z);
			D3DXVECTOR3 point3 = D3DXVECTOR3(c.position.x, c.position.y, c.position.z);

			Triangle toAdd = Triangle(point1, point2, point3);
			int pos = (ceilf(toAdd.centerPos.x / 256.0f)) + (floorf(toAdd.centerPos.z / 256.0f) * 4);

 			switch (pos) {
			case 1:
				terrainCheck1.push_back(Triangle(point1, point2, point3));
				break;
			case 2:
				terrainCheck2.push_back(Triangle(point1, point2, point3));
				break;
			case 3: 
				terrainCheck3.push_back(Triangle(point1, point2, point3));
				break;
			case 4:
				terrainCheck4.push_back(Triangle(point1, point2, point3));
				break;
			case 5:
				terrainCheck5.push_back(Triangle(point1, point2, point3));
				break;
			case 6:
				terrainCheck6.push_back(Triangle(point1, point2, point3));
				break;
			case 7:
				terrainCheck7.push_back(Triangle(point1, point2, point3));
				break;
			case 8:
				terrainCheck8.push_back(Triangle(point1, point2, point3));
				break;
			case 9:
				terrainCheck9.push_back(Triangle(point1, point2, point3));
				break;
			case 10:
				terrainCheck10.push_back(Triangle(point1, point2, point3));
				break;
			case 11:
				terrainCheck11.push_back(Triangle(point1, point2, point3));
				break;
			case 12:
				terrainCheck12.push_back(Triangle(point1, point2, point3));
				break;
			case 13:
				terrainCheck13.push_back(Triangle(point1, point2, point3));
				break;
			case 14:
				terrainCheck14.push_back(Triangle(point1, point2, point3));
				break;
			case 15:
				terrainCheck15.push_back(Triangle(point1, point2, point3));
				break;
			case 16:
				terrainCheck16.push_back(Triangle(point1, point2, point3));
				break;
			}
		}
	}

	return true;
}

float CollisionMap::GetHeight(Car * car)
{
	D3DXVECTOR3 carPos = car->GetPosition();
	for (int i = 0; i < trackCheck.size(); i++) {
		if (trackCheck[i].isWithin(carPos)) {
			return trackCheck[i].centerPos.y + 0.3f;
		}
	}

	return 0.0f;
}

bool CollisionMap::CheckCollision(Car * car)
{
	D3DXVECTOR3 carPos = car->GetPosition();
	int pos = (ceilf(carPos.x / 256.0f)) + (floorf(carPos.z / 256.0f) * 4);

	float halfWidth = (car->m_Model->GetWidth() / 2) * car->scale;
	float halfLength = (car->m_Model->GetLength() / 2) * car->scale;
	UL = carPos + (car->GetLeftVector() * halfWidth) + (car->GetForwardVector() * halfLength);
	BL = UL + (-car->GetForwardVector() * (halfLength * 2));
	UR = UL + (car->GetRightVector() * (halfWidth * 2));
	BR = UR + (-car->GetForwardVector() * (halfLength * 2));

	switch (pos) {
	case 1:
		for (int i = 0; i < terrainCheck1.size(); i++) {
			if (terrainCheck1[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck1[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck1[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck1[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 2:
		for (int i = 0; i < terrainCheck2.size(); i++) {
			if (terrainCheck2[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck2[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck2[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck2[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 3:
		for (int i = 0; i < terrainCheck3.size(); i++) {
			if (terrainCheck3[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck3[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck3[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck3[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 4:
		for (int i = 0; i < terrainCheck4.size(); i++) {
			if (terrainCheck4[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck4[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck4[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck4[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 5:
		for (int i = 0; i < terrainCheck5.size(); i++) {
			if (terrainCheck5[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck5[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck5[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck5[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 6:
		for (int i = 0; i < terrainCheck6.size(); i++) {
			if (terrainCheck6[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck6[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck6[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck6[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 7:
		for (int i = 0; i < terrainCheck7.size(); i++) {
			if (terrainCheck7[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck7[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck7[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck7[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 8:
		for (int i = 0; i < terrainCheck8.size(); i++) {
			if (terrainCheck8[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck8[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck8[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck8[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 9:
		for (int i = 0; i < terrainCheck9.size(); i++) {
			if (terrainCheck9[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck9[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck9[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck9[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 10:
		for (int i = 0; i < terrainCheck10.size(); i++) {
			if (terrainCheck10[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck10[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck10[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck10[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 11:
		for (int i = 0; i < terrainCheck11.size(); i++) {
			if (terrainCheck11[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck11[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck11[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck11[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 12:
		for (int i = 0; i < terrainCheck12.size(); i++) {
			if (terrainCheck12[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck12[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck12[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck12[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 13:
		for (int i = 0; i < terrainCheck13.size(); i++) {
			if (terrainCheck13[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck13[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck13[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck13[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 14:
		for (int i = 0; i < terrainCheck14.size(); i++) {
			if (terrainCheck14[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck14[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck14[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck14[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 15:
		for (int i = 0; i < terrainCheck15.size(); i++) {
			if (terrainCheck15[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck15[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck15[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck15[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	case 16:
		for (int i = 0; i < terrainCheck16.size(); i++) {
			if (terrainCheck16[i].isWithin(UL)) {
				return true;
			}
			else if (terrainCheck16[i].isWithin(BL)) {
				return true;
			}
			else if (terrainCheck16[i].isWithin(UR)) {
				return true;
			}
			else if (terrainCheck16[i].isWithin(BR)) {
				return true;
			}
		}
		break;
	}

	return false;
}

int CollisionMap::CheckPoint(Car * car)
{
	int cpCollided = -1;
	for (int i = 0; i < checkPointsCheck.size(); i++) {
		if ((checkPointsCheck[i].isWithin(UR)) || (checkPointsCheck[i].isWithin(UL)) || (checkPointsCheck[i].isWithin(BL)) || (checkPointsCheck[i].isWithin(BR))) {
			cpCollided = i;
			break;
		}
	}

	if (cpCollided == -1) {
		return cpCollided;
	}

	for (int i = 0; i < noOfCheckpoints; i++) {
		if ((cpCollided >= (i * 2)) && (cpCollided < ((i + 1) * 2))) {
			return i;
		}
	}
}
