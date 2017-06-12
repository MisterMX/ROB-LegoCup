#pragma config(Sensor, S1,     sensorColorLeft, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2,     sensorColorRight, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3,     sensorUltrasonic, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          right,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          rotor,         tmotorEV3_Medium, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "colors.h"
#include "colors.c"

#define STATE_DEFAULT 0
#define STATE_TURN_LEFT 1
#define STATE_TURN_RIGHT 2
#define STATE_OBSTACLE 3
#define STATE_SEARCH_TARGET_AREA 4
#define STATE_SEARCH_BALLS 5
#define STATE_END -1

#define SPEED_STRAIGHT -25
#define SPEED_TURN 20

#define POWER_STRAIGHT 0
#define POWER_TURN_LEFT -100
#define POWER_TURN_RIGHT 100

TLegoColors lastLeftColor;
TLegoColors lastRightColor;

int currentState = STATE_SEARCH_BALLS;

// predefined methods
void stateDefault();
void stateTurnLeft();
void stateTurnRight();

void stateDefault()
{
		TLegoColors colorLeft = getLegoColorFromRGB(sensorColorLeft, 2);
		TLegoColors colorRight = getLegoColorFromRGB(sensorColorRight, 3);

		if (colorLeft == colorWhite
			&& colorRight == colorWhite)
		{
			// drive straight
			displayTextLine(4, "2 white");
			setMotorSync(left, right, POWER_STRAIGHT, SPEED_STRAIGHT);
		}
		else if(colorLeft == colorBlack
			&& colorRight == colorBlack)
		{
			displayTextLine(4, "2 black");

			if(lastRightColor == colorBlack && lastLeftColor != colorBlack)
				setMotorSync(left, right, 100, SPEED_TURN);
			else if( lastRightColor != colorBlack && lastLeftColor == colorBlack)
				setMotorSync(left, right, -100, SPEED_TURN);
			else
				setMotorSync(left,right, POWER_STRAIGHT, SPEED_STRAIGHT);
		}
		else if (colorLeft == colorGreen)
		{
			setMotorSync(left,right, POWER_STRAIGHT, 0);
			if(lastLeftColor == colorGreen){
				// turn left 90 degrees
				currentState = STATE_TURN_LEFT;
			}
		}
		else if (colorRight == colorGreen)
		{
			setMotorSync(left,right, POWER_STRAIGHT, 0);
			if(lastRightColor == colorGreen){
				// turn right 90 degrees
				currentState = STATE_TURN_RIGHT;
			}

		}
		else if (colorLeft == colorBlack)
		{
			// turn right
			displayTextLine(4, "LeftBlack");
			setMotorSync(left, right, POWER_TURN_RIGHT, SPEED_TURN);
		}
		else if (colorRight == colorBlack)
		{
			// turn left
			displayTextLine(4, "RightBlack");
			setMotorSync(left, right, POWER_TURN_LEFT, SPEED_TURN);
		}
		else if(colorRight == colorYellow && colorLeft == colorYellow){
			currentState = STATE_SEARCH_BALLS;
		}

		lastLeftColor = colorLeft;
		lastRightColor = colorRight;

		float distanze = getUSDistance(sensorUltrasonic);
		if(distanze < 8)
		{
			currentState = STATE_OBSTACLE;
		}
}

void stateTurnLeft()
{
	// Move the center of the robot to the mid of the crossing.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 60, SPEED_STRAIGHT/2);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

	TLegoColors colorLeft = getLegoColorFromRGB(sensorColorLeft, 2);
	if( colorLeft == colorWhite){
		currentState = STATE_DEFAULT;
			// Turn right.
		setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 35, SPEED_TURN);
		while (getMotorRunning(left) != 0)
			displayString(2, "Left encoder: %d", getMotorEncoder(left));
	}
	else
	{
		setMotorSyncEncoder(left, right, POWER_STRAIGHT, 100, SPEED_STRAIGHT);
		while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));

		// Turn left.
		setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 535, SPEED_TURN);
		while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));

		// Drive off the crossing.
		setMotorSyncEncoder(left, right, POWER_STRAIGHT, 70, SPEED_STRAIGHT);
		while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));

		currentState = STATE_DEFAULT;
	}
}

void stateTurnRight()
{
	// Move the center of the robot to the mid of the crossing.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 60, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

	TLegoColors colorRight = getLegoColorFromRGB(sensorColorRight, 3);
	if( colorRight == colorWhite)
	{
		currentState = STATE_DEFAULT;
		// Turn left.
		setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 35, SPEED_TURN);
		while (getMotorRunning(right) != 0)
			displayString(2, "Left encoder: %d", getMotorEncoder(right));
	}
	else
	{
		// Move the center of the robot to the mid of the crossing.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 100, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

	// Turn right.
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 535, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

	// Drive off the crossing.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 70, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

	currentState = STATE_DEFAULT;
	}
}

void stateObstacle()
{
	// Turn left.
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 535, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

	// Move away from line.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 400, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

	// Turn Right.
	setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 535, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

		// Move on the obstacle side.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 1200, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

		// Turn Right.
	setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 535, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

		// Move back to line.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 400, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

		// Turn left.
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 535, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

	currentState = STATE_DEFAULT;
}

void searchTargetArea()
{

}

void raiseBalls()
{
	resetMotorEncoder(rotor);
	setMotorTarget(rotor,90,10);
}

void catchBall()
{
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 300, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));
	resetMotorEncoder(rotor);
	setMotorTarget(rotor,180,50);
}

void searchBalls()
{

	raiseBalls();
	/*
	float distanze = getUSDistance(sensorUltrasonic);
	if(distanze < 8){
		// Turn Right.
		setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 150, SPEED_TURN);
		while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));

		distanze = getUSDistance(sensorUltrasonic);
		displayString(5, "Distance: %d", distanze);

			// Turn left.
		setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 150, SPEED_TURN);
		while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));

		if(distanze > 10){
			catchBall();
		}
	}*/
	//currentState = STATE_DEFAULT;
}

task main()
{
	bool run = true;
	while (run)
	{
		switch (currentState)
		{
			case STATE_DEFAULT:
				displayTextLine(1, "STATE: Default");
				stateDefault();
				break;

			case STATE_TURN_LEFT:
				displayTextLine(1, "STATE: Turn left");
				stateTurnLeft();
				break;

			case STATE_TURN_RIGHT:
				displayTextLine(1, "STATE: Turn right");
				stateTurnRight();
				break;

			case STATE_OBSTACLE:
				displayTextLine(1, "STATE: Obstacle");
				stateObstacle();
				break;

			case STATE_SEARCH_TARGET_AREA:
				displayTextLine(1, "STATE: Search target area");
				searchTargetArea();
				break;

			case STATE_SEARCH_BALLS:
				displayTextLine(1, "STATE: Search Balls");
				searchBalls();
				break;

			case STATE_END:
				displayTextLine(1, "STATE: End");
				run = false;
				break;
		}
	}
}
