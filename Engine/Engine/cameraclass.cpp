////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	distance = 6.0f; //Distance to keep from car
	height = 1.0f; //Height from ground
	springConstant = 160.0f; //How much the camera should try to get to its ideal position
	dampConstant = 2.0f * sqrt(springConstant); //Dampening factor for the springyness
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


void CameraClass::Render()
{
	// Position of the camera
	D3DXVECTOR3 position;
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	D3DXVECTOR3 lookAt;
	lookAt = carPos + up * height;

	// View matrix generated from the position of the camera, the position of the car, and the 'up' vector
	D3DXMatrixLookAtLH(&m_viewMatrix, &-position, &lookAt, &up);

	return;
}

void CameraClass::RenderPreScene()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::GetReverseViewMatrix(D3DXMATRIX &viewMatrix)
{
	//Find Camera position
	D3DXVECTOR3 position;
	position = -GetPosition();

	//Find vector between camera and car
	D3DXVECTOR3 lookAt;
	D3DXVec3Subtract(&lookAt, &carPos, &position);
	//Find a 'lookat' vector behind the camera by subtracting vector to car from camera postion
	lookAt = position - lookAt;
	//Subtract camera height from 'lookat' vector
	lookAt = lookAt - up * height;

	//Construct new viewing matrix
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}

void CameraClass::Follow(D3DXVECTOR3 followTarget, D3DXVECTOR3 targetForwardVector, float deltaTime)
{
	//Update car position for view matrix calculation
	carPos = followTarget;

	//Ideal position equals target position plus distance and height 
	D3DXVECTOR3 idealPosition = followTarget - targetForwardVector * distance + up * height;

	//Find the distance between current position and desired position
	D3DXVECTOR3 myPosition = GetPosition();
	D3DXVECTOR3 displacement = myPosition + idealPosition;

	//Calculation to determine velocity using the spring factor and displacement
	D3DXVECTOR3 springAcceleration = (-springConstant * displacement) - (dampConstant * velocity);

	//Apply velocity
	velocity += springAcceleration * deltaTime;
	SetPosition(myPosition.x + (velocity.x * deltaTime), myPosition.y + (velocity.y * deltaTime), myPosition.z + (velocity.z * deltaTime));
}