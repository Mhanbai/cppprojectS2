#include "car.h"

Car::Car()
{
	m_Model = 0;

	// Set up gameplay variables
	accelerationInput = 0.0f;
	steerInput = 0.0f;
	graphicsAngle = 0.0f;
	startAccelerationFactor = 10.0f;
	gear = 0.5f;
	startingForwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	forwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	upVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	acceleration = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isAccelerating = false;
	isTurningLeft = false;
	isTurningRight = false;
	isBreakReversing = false;

	//Car handling
	frictionFactor = 0.5f;
	lateralFrictionFactor = 3.0f;
	steerFactor = 1.1f;
}

Car::Car(const Car &)
{
}

Car::~Car()
{
}

bool Car::Initialize(char* modelFilename, WCHAR* textureFilename, ModelClass* &model_in, ID3D11Device* device)
{
	bool result;

	m_Model = new ModelClass;
	m_Model->Initialize(device, modelFilename, textureFilename);

	model_in = m_Model;

	return true;
}

void Car::Shutdown()
{
	m_Model = 0;
}

void Car::Frame(float deltaTime)
{
	//The speed of the car is equivalent to the magnitude of the velocity vector
	speed = (D3DXVec3Length(&velocity));

	//This handles both gear changing and car noise
	if (speed < 30.0f) {
		gear = 0.5f;
	}
	else if ((speed >= 30.0f) && (speed < 60.0f)) {
		gear = 0.75f;
	}
	else {
		gear = 1.0f;
	}

	//Controls the 'gear' i.e. the car moves at a speed relevant to how fast its already going
	accelerationFactor = startAccelerationFactor + (speed * gear);

	//User Input
	if (isAccelerating) {
		accelerationInput = 1.0f;
	}
	else if (isBreakReversing) {
		accelerationInput = -0.5f;
	}
	else {
		accelerationInput = 0.0f;
	}

	if (isTurningLeft) {
		if (isBreakReversing) {
			steerInput = 1.0f;
		}
		else {
			steerInput = -1.0f;
		}
	}
	else if (isTurningRight) {
		if (isBreakReversing) {
			steerInput = -1.0f;
		}
		else {
			steerInput = 1.0f;
		}
	}
	else {
		steerInput = 0.0f;
	}

	//Angle of steering equals user input multiplied by how well car handles
	steerAngle = steerInput * steerFactor * deltaTime;

	//Divide the angle by speed divided by 100. This stops the car from being able to turn on the spot
	steerAngle = steerAngle * (speed / 100.0f);

	//Calculate new forward vector
	D3DXMatrixRotationY(&rotation, steerAngle); //Create a matrix for rotation around Y from angle of steering
	D3DXVec3Transform(&nextForwardVector, &forwardVector, &rotation); //Transform forward vector by rotation matrix
	forwardVector = D3DXVECTOR3(nextForwardVector.x, nextForwardVector.y, nextForwardVector.z); //As Vec3Transform returns a Vector4, put these values into a Vector3

	//Calculate new right vector from forward vector
	D3DXVec3Normalize(&forwardVectorNormalized, &forwardVector);
	D3DXVec3Cross(&rightVector, &forwardVectorNormalized, &upVector);

	//Lateral friction stops the car from drifting as if it is on ice
	lateralVelocity = rightVector * D3DXVec3Dot(&velocity, &rightVector);
	lateralFriction = -lateralVelocity * lateralFrictionFactor;

	//Acceleration is equal to direction car is facing multiplied by the rate of acceleration multiplied by input
	acceleration = forwardVector * accelerationInput * accelerationFactor;

	//Friction is equal to the reverse of velocity multiplied by how frictiony the surface is
	friction = -velocity * frictionFactor;
	velocity += (friction + lateralFriction) * deltaTime;

	//Increase velocity by acceleration
	if (speed < 100.0f) {
		velocity += acceleration * deltaTime;
	}

	//Apply velocity to car position
	position = position + velocity * deltaTime;

	//Calculate angle car is facing for graphics
	graphicsAngle = atan2(forwardVector.z, forwardVector.x) - atan2(startingForwardVector.z, startingForwardVector.x);

	//Set the position of the cars model
	m_Model->Transform(position, graphicsAngle);
}

void Car::Accelerate(bool set)
{
	if (set == true) {
		isAccelerating = true;
	}
	else {
		isAccelerating = false;
	}
}

void Car::BreakReverse(bool set)
{
	if (set == true) {
		isBreakReversing = true;
	}
	else {
		isBreakReversing = false;
	}
}

void Car::TurnLeft(bool set)
{
	if (set == true) {
		isTurningLeft = true;
	}
	else {
		isTurningLeft = false;
	}
}

void Car::TurnRight(bool set)
{
	if (set == true) {
		isTurningRight = true;
	}
	else {
		isTurningRight = false;
	}
}

void Car::SetPosition(D3DXVECTOR3 position_in, float angle_in)
{
	position = position_in;
	m_Model->Transform(position, angle_in);
}

D3DXVECTOR3 Car::GetForwardVector()
{
	return forwardVector;
}

D3DXVECTOR3 Car::GetPosition()
{
	return position;
}

bool Car::GetLateralPosition(D3DXVECTOR3 toTest, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2)
{
	//This calculation returns a negative number if 'toTest' is to the left of the two line points, and a positive number or zero if it is to the right
	float orientation = ((linePoint2.x - linePoint1.x) * (toTest.z - linePoint1.z)) - ((linePoint2.z - linePoint1.z) * (toTest.x - linePoint1.x));
	if (orientation >= 0.0f) {
		return true; //Point is to the right
	}
	else {
		return false; //Point is to the left
	}
}
