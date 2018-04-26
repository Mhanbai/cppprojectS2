#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: car.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAR_H_
#define _CAR_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <windows.h>
#include <time.h>
#include <vector>
#include "d3dclass.h"
#include "modelclass.h"
#include "textclass.h"
#include "d3dclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Car
////////////////////////////////////////////////////////////////////////////////
class Car
{
public:
	//Contains vectors that represent the cars 4 surrounding positions in terms of X & Z coordinates
	struct CollisionBox {
		D3DXVECTOR3 frontLeft;
		D3DXVECTOR3 frontRight;
		D3DXVECTOR3 backLeft;
		D3DXVECTOR3 backRight;
		CollisionBox()
			: frontLeft(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), 
			frontRight(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), 
			backLeft(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), 
			backRight(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
		}
	};

	Car();
	Car(const Car&);
	~Car();
	bool Initialize(char* modelFilename, WCHAR* textureFilename, ModelClass* &model_in, ID3D11Device* device, D3DXVECTOR3 startingDirection, TextClass* text_in, ID3D11DeviceContext* deviceContext_in);
	void Shutdown();
	void Frame(float deltaTime);
	void OpponentFrame(float deltaTime);
	void SetRacingLine(std::vector<D3DXVECTOR3> racingLine);

	void Accelerate(bool);
	void BreakReverse(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void SetPosition(D3DXVECTOR3 position_in);
	D3DXVECTOR3 GetForwardVector();
	D3DXVECTOR3 GetRightVector();
	D3DXVECTOR3 GetLeftVector();
	D3DXVECTOR3 GetPosition();

	ModelClass* m_Model;
	TextClass* m_Text;
	ID3D11DeviceContext* deviceContext;

	bool colliding = false;
	void SetColliding(bool set);
	float scale = 0.3;

	int currentCheckPoint = 0;

	D3DXVECTOR3 debug[2];

private:
	//Useful vectors for determining positions
	D3DXVECTOR3 startingForwardVector;
	D3DXVECTOR3 forwardVector;
	D3DXVECTOR4 nextForwardVector;
	D3DXVECTOR3 oppNextForwardVector;
	D3DXVECTOR3 forwardVectorNormalized;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 upVector;

	std::vector<D3DXVECTOR3> m_RacingLine;
	int currentNode = 1;
	D3DXVECTOR3 emptyNode = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//int node = 0;
	float DistanceFromLine(D3DXVECTOR3 position, D3DXVECTOR3 horizontalLine, D3DXVECTOR3 prevNode, D3DXVECTOR3 nextNode);
	D3DXVECTOR3 CalculateLine(D3DXVECTOR3 point1, D3DXVECTOR3 point2);
	D3DXVECTOR3 FindIntersectionPoint(D3DXVECTOR3 line1, D3DXVECTOR3 line2);
	int checkPoint = 0;
	float integral = 0.0f;
	float previousError = 0.0f;
	float kp = 1.0f;
	float ki = 0.4f;
	float kd = 0.1f;

	//Velocity, friction, steering etc...
	D3DXVECTOR3 velocity = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 friction;
	D3DXVECTOR3 acceleration;
	float speed;
	float accelerationFactor;
	float startAccelerationFactor;
	float gear;
	float frictionFactor;
	float accelerationInput;
	float steerInput;
	float steerFactor;
	float steerAngle;
	float steeringScalar;
	float topSpeed;
	float gearRange;

	D3DXVECTOR3 previousForwardVector;
	float angleDelta, angleTracker;
	int counter = 0;

	//Values for lateral velocity
	D3DXVECTOR3 lateralVelocity;
	D3DXVECTOR3 lateralFriction;
	float lateralFrictionFactor;

	//Rotation matrix
	D3DXMATRIX rotation;

	//Position of model for graphics & game
	D3DXVECTOR3 position;
	float angleOffset;

	//Booleans for user input
	bool isAccelerating;
	bool isTurningLeft;
	bool isTurningRight;
	bool isBreakReversing;

	//Functions, lists and variables used to help with checking the car is on the track
	bool IsLeft(D3DXVECTOR3 pointA, D3DXVECTOR3 pointB, D3DXVECTOR3 pointC);
};

#endif