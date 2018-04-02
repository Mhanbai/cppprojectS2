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
	bool Initialize(char* modelFilename, WCHAR* textureFilename, ModelClass* &model_in, ID3D11Device* device);
	void Shutdown();
	void Frame(float deltaTime);

	void Accelerate(bool);
	void BreakReverse(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void SetPosition(D3DXVECTOR3 position_in, float angle_in);
	D3DXVECTOR3 GetForwardVector();
	D3DXVECTOR3 GetPosition();

	ModelClass* m_Model;

private:
	//Useful vectors for determining positions
	D3DXVECTOR3 startingForwardVector;
	D3DXVECTOR3 forwardVector;
	D3DXVECTOR4 nextForwardVector;
	D3DXVECTOR3 forwardVectorNormalized;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 upVector;

	//Velocity, friction, steering etc...
	D3DXVECTOR3 velocity;
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

	//Values for lateral velocity
	D3DXVECTOR3 lateralVelocity;
	D3DXVECTOR3 lateralFriction;
	float lateralFrictionFactor;

	//Rotation matrix
	D3DXMATRIX rotation;

	//Position of model for graphics & game
	D3DXVECTOR3 position;
	float graphicsAngle;

	//Booleans for user input
	bool isAccelerating;
	bool isTurningLeft;
	bool isTurningRight;
	bool isBreakReversing;

	//Functions, lists and variables used to help with checking the car is on the track
	bool GetLateralPosition(D3DXVECTOR3 toTest, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2);
};

#endif