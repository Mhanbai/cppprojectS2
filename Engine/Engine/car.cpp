#include "car.h"

Car::Car()
{
	m_Model = 0;

	// Set up gameplay variables
	topSpeed = 25.0f;
	accelerationInput = 0.0f;
	steerInput = 0.0f;
	angleOffset = 0.0f;
	startAccelerationFactor = 2.0f;
	gear = 1.0f;
	upVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	acceleration = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gearRange = topSpeed / 5.0f;
	isAccelerating = false;
	isTurningLeft = false;
	isTurningRight = false;
	isBreakReversing = false;

	//Car handling
	frictionFactor = 0.5f;
	lateralFrictionFactor = 2.0f;
	steerFactor = 1.1f;
}

Car::Car(const Car &)
{
}

Car::~Car()
{
}

bool Car::Initialize(char* modelFilename, WCHAR* textureFilename, ModelClass* &model_in, ID3D11Device* device, D3DXVECTOR3 startingDirection, TextClass* text_in, ID3D11DeviceContext* deviceContext_in)
{
	bool result;

	m_Model = new ModelClass;
	m_Model->Initialize(device, modelFilename, textureFilename);
	m_Model->Scale(0.3f);

	model_in = m_Model;

	startingForwardVector = startingDirection;
	forwardVector = startingDirection;
	previousForwardVector = startingDirection;

	//Find starting direction for model
	angleOffset = atan2(forwardVector.z, forwardVector.x) - atan2(-1.0f, 0.0f);
	m_Model->Transform(position, -angleOffset);

	m_Text = text_in;
	deviceContext = deviceContext_in;

	return true;
}

void Car::Shutdown()
{
	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
}

void Car::Frame(float deltaTime)
{
	//The speed of the car is equivalent to the magnitude of the velocity vector
	speed = (D3DXVec3Length(&velocity));

	//This handles both gear changing and car noise
	if (speed < (gearRange)) {
		gear = 0.2f;
	}
	else if ((speed >= (gearRange)) && (speed < (gearRange * 2))) {
		gear = 0.4f;
	}
	else if ((speed >= (gearRange * 2)) && (speed < (gearRange * 3))) {
		gear = 0.6f;
	}
	else if ((speed >= (gearRange * 3)) && (speed < (gearRange * 4))) {
		gear = 0.8f;
	}
	else  {
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
	steerAngle = steerAngle * (speed / topSpeed);

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
	if (speed < topSpeed) {
		velocity += acceleration * deltaTime;
	}

	//Apply velocity to car position
	position = position + velocity * deltaTime;

	//Set the position of the cars model
	m_Model->Transform(position, steerAngle);
}

void Car::OpponentFrame(float deltaTime)
{
	float pid;
	float speed = 3.0f;

	D3DXVec3Subtract(&forwardVector, &m_RacingLine[currentNode + 1], &m_RacingLine[currentNode]);

	position += forwardVector * deltaTime * speed;
	emptyNode += forwardVector * deltaTime * speed;

	if (D3DXVec3Length(&emptyNode) > D3DXVec3Length(&forwardVector)) {
		currentNode++;
		emptyNode = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//Find distance from racing line
	////Calculate vectors from left and right of car
	D3DXVec3Normalize(&forwardVector, &forwardVector);
	D3DXVECTOR3 tangentLine = D3DXVECTOR3(forwardVector.z, forwardVector.y, -forwardVector.x);
	////Find distance to racing line
	float distance = DistanceFromLine(position, position + tangentLine, m_RacingLine[currentNode], m_RacingLine[currentNode + 1]);

	//Calculate error
	float error = distance;

	//Calculate integral
	integral = integral + (error * deltaTime);

	if (error == 0.0f) {
		integral = 0.0f;
	}

	if (abs(integral) > 1.0f) {
		integral = 0.0f;
	}

	//Calculate derivative
	float derivative = ((error - previousError) * deltaTime);
	previousError = error;

	//Steering angle equals error + inegral + derivative clamped between 1 and -1
	pid = (error * kp) + (integral * ki) + (derivative * kd);

	position += tangentLine * pid * speed * deltaTime;

	angleDelta = atan2(forwardVector.z, forwardVector.x) - atan2(previousForwardVector.z, previousForwardVector.x);
	previousForwardVector = forwardVector;

	m_Model->Transform(position, -angleDelta);
}

void Car::SetRacingLine(std::vector<D3DXVECTOR3> racingLine)
{
	m_RacingLine = racingLine;
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

void Car::SetPosition(D3DXVECTOR3 position_in)
{
	position = position_in;
	m_Model->Transform(position, 0.0f);
}

D3DXVECTOR3 Car::GetForwardVector()
{
	return forwardVector;
}

D3DXVECTOR3 Car::GetPosition()
{
	return position;
}

float Car::DistanceFromLine(D3DXVECTOR3 position, D3DXVECTOR3 horizontalLine, D3DXVECTOR3 prevNode, D3DXVECTOR3 nextNode)
{
	//Find A, B and C for lines
	D3DXVECTOR3 horLine = CalculateLine(position, horizontalLine);

	//Find A, B and C for current 2 trackpoints
	D3DXVECTOR3 trackLine = CalculateLine(prevNode, nextNode);

	//Find where car lines intersect with track line
	D3DXVECTOR3 intersectionPoint = FindIntersectionPoint(horLine, trackLine);

	//If intersection point is on either right or left line, return distance to point
	if (IsLeft(intersectionPoint, position, position + forwardVector)) {
		D3DXVECTOR3 vectorFromLine = intersectionPoint - position;
		return -D3DXVec3Length(&vectorFromLine);
	} else {
		D3DXVECTOR3 vectorFromLine = intersectionPoint - position;
		return D3DXVec3Length(&vectorFromLine);
	}
}

D3DXVECTOR3 Car::CalculateLine(D3DXVECTOR3 point1, D3DXVECTOR3 point2)
{
	float A = point2.z - point1.z;
	float B = point1.x - point2.x;
	float C = (A * point1.x) + (B * point1.z);

	return D3DXVECTOR3(A, B, C);
}

D3DXVECTOR3 Car::FindIntersectionPoint(D3DXVECTOR3 line1, D3DXVECTOR3 line2)
{
	float det = (line1.x * line2.y) - (line2.x * line1.y);

	float x = ((line2.y * line1.z) - (line1.y * line2.z)) / det;
	float z = ((line1.x * line2.z) - (line2.x * line1.z)) / det;

	return D3DXVECTOR3(x, 0.0f, z);
}

bool Car::IsLeft(D3DXVECTOR3 pointA, D3DXVECTOR3 pointB, D3DXVECTOR3 pointC)
{
	return ((pointB.x - pointA.x)*(pointC.z - pointA.z)) - ((pointB.z - pointA.z) * (pointC.x - pointA.x)) > 0;
}
