#pragma config(Sensor, S1,     sensorColorLeft, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2,     sensorGyro,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     sensorUltrasonic, sensorEV3_Ultrasonic)
#pragma config(Sensor, S4,     sensorColorRight, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor,  motorA,          left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          right,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          rotor,         tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorD,          catchMotor,    tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "colors.h"
#include "colors.c"

#define UINT unsigned int

#define STATE_CALIBRATE_GYRO 90
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
#define ARM_DRIVE_UP -850
#define ARM_DRIVE_DOWN 850
#define CATCH_BALL_POSITION 1100
#define DROP_BALL_POSITION -1100

#define US_SENSOR_BUFFER_SIZE 3

TLegoColors lastLeftColor;
TLegoColors lastRightColor;

int currentState = STATE_CALIBRATE_GYRO;

long lastDegrees = 0;

// predefined methods
void stateDefault();
void stateTurnLeft();
void stateTurnRight();
void dropBalls();
void shiftArray(float* array, const UINT size);
void driveToNextWall();
bool checkIsWall();

bool armUp = false;
float rotorSensorValues[US_SENSOR_BUFFER_SIZE];

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

		//float distance2 = getUSDistance(sensorUltrasonicHorizontal);
		float distanze = getUSDistance(sensorUltrasonic);
		displayString(7, "Distance D: %f", distanze);
		//displayString(8, "Distance H: %f", distance2);
		shiftArray(rotorSensorValues, US_SENSOR_BUFFER_SIZE);
		rotorSensorValues[0] = distanze;

		bool driveUp = true;
		for(int i = 0; i < US_SENSOR_BUFFER_SIZE; i++){
			if(rotorSensorValues[i] < 250)
			{
				driveUp = false;
			}
		}

		if (time1[T1] > 1000)
		{

		long degrees = getGyroDegrees(sensorGyro) % 90;
		//long heading = getGyroHeading(sensorGyro) % 90;
		//long rate = getGyroRate(sensorGyro);

		displayString(8, "degrees: %ld", degrees);
		//displayString(8, "heading: %ld", heading);
		//displayString(9, "rate: %ld", rate);

		long degreesDiff = degrees - lastDegrees;
		displayString(9, "degrees: %ld", degreesDiff);

		if (degreesDiff < -10 && !armUp)
		{
				// move arm up
			setMotorSync(left,right,0,0);
			armUp = true;
			resetMotorEncoder(rotor);
			setMotorTarget(rotor , 500, 20);
			while(getMotorRunning(rotor))
				displayString(4,"rotor drive up");
		}
		else if (degreesDiff > 10  && armUp)
		{
			setMotorSync(left,right,0,0);

			setMotorSyncEncoder(left, right, POWER_STRAIGHT, 100, SPEED_STRAIGHT/2);
			while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));


			armUp = false;
			resetMotorEncoder(rotor);
			// move arm down
			setMotorTarget(rotor,-500, 20);
			while(getMotorRunning(rotor))
				displayString(4,"rotor drive down");

				currentState = STATE_CALIBRATE_GYRO;
		}

		lastDegrees = degrees;

		clearTimer(T1);
	}

		if(distanze < 4)
		{
			currentState = STATE_OBSTACLE;
		}
		/* else if(driveUp && !armUp)
		{
			setMotorSync(left,right,0,0);
			armUp = true;
			resetMotorEncoder(rotor);
			setMotorTarget(rotor,ARM_DRIVE_DOWN/2, 20);
			while(getMotorRunning(rotor)
				displayString(4,"rotor drive up");
		}
		else if(rotorSensorValues[0] < 90 && rotorSensorValues[1] < 90 && armUp)
		{
			setMotorSync(left,right,0,0);
			armUp = false;
			resetMotorEncoder(rotor);
			setMotorTarget(rotor,ARM_DRIVE_UP/2, 20);
			while(getMotorRunning(rotor)
				displayString(4,"rotor drive up");
		}*/

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
		setMotorSyncEncoder(left, right, POWER_STRAIGHT, 120, SPEED_STRAIGHT);
		while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));

		// Turn left.
		setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 600, SPEED_TURN);
		bool getLine = false;
		while (getMotorRunning(right) != 0)
		{
			TLegoColors colorLeft = getLegoColorFromRGB(sensorColorLeft, 3);
			if(colorLeft == colorBlack)
			{
				getLine = true;
			}
			else if(colorLeft == colorWhite && getLine)
			{
				setMotorSync(right, left, 0,0);
				break;
			}
		}

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
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 120, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

	// Turn right.
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 600, SPEED_TURN);
	bool getLine = false;
	while (getMotorRunning(left) != 0)
	{
		TLegoColors colorRight = getLegoColorFromRGB(sensorColorRight, 3);
		if(colorRight == colorBlack)
		{
			getLine = true;
		}
		else if(colorRight == colorWhite && getLine)
		{
				setMotorSync(right, left, 0,0);
				break;
		}
	}

	currentState = STATE_DEFAULT;
	}
}

void stateObstacle()
{
	// Turn left.
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 500, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

	// Move away from line.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 500, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

	// Turn Right.
	setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 500, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

		// Move on the obstacle side.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 1500, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

		// Turn Right.
	setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 500, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));


		// Move back to line.
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 1200, SPEED_STRAIGHT);
	bool getLine = false;
	while (getMotorRunning(left) != 0)
	{
		TLegoColors colorLeft = getLegoColorFromRGB(sensorColorLeft, 3);
		if(colorLeft == colorBlack)
		{
				getLine = true;
		}
		if(getLine && colorLeft == colorWhite)
		{
			break;
		}
	}

	// Turn Right.
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 100, SPEED_TURN);
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
	setMotorTarget(rotor,ARM_DRIVE_DOWN,50);

	while(getMotorRunning(rotor))
			displayString(2,"Running");

	resetMotorEncoder(catchMotor);
	setMotorTarget(catchMotor,CATCH_BALL_POSITION,100);

	while(getMotorRunning(catchMotor))
			displayString(2,"Running");

	resetMotorEncoder(rotor);
	setMotorTarget(rotor,ARM_DRIVE_UP,50);

	while(getMotorRunning(rotor))
			displayString(2,"Running");

	dropBalls();
}

void dropBalls()
{
	resetMotorEncoder(rotor);
	setMotorTarget(rotor,ARM_DRIVE_DOWN,50);

	while(getMotorRunning(rotor))
			displayString(2,"Running");

	resetMotorEncoder(catchMotor);
	setMotorTarget(catchMotor,DROP_BALL_POSITION,100);

	while(getMotorRunning(catchMotor))
			displayString(2,"Running");

	resetMotorEncoder(rotor);
	setMotorTarget(rotor,ARM_DRIVE_UP,50);

	while(getMotorRunning(rotor))
			displayString(2,"Running");

	currentState = STATE_END;
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
	setMotorSync(left,right,0, 0);
	setMotorSyncEncoder(left, right, POWER_STRAIGHT, 500, SPEED_STRAIGHT);
	while (getMotorRunning(left) != 0)
		displayString(2, "Left encoder: %d", getMotorEncoder(left));

			// Turn Right.
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 500, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));

	int radius = 0;
	while(radius < 800)
	{
			setMotorSyncEncoder(left, right, POWER_TURN_RIGHT, 20, SPEED_TURN);
			while (getMotorRunning(left) != 0)
				displayString(4, "Left encoder: %d", getMotorEncoder(left));
			float distanze = getUSDistance(sensorUltrasonic);
			if(distanze < 35 )
				break;
			displayString(9,"Distance: %f", distanze);
			radius += 20;
	}
	driveToNextWall();

	currentState = STATE_END;

	//raiseBalls();
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

void driveToNextWall()
{
	bool search = true;
	while(search)
	{
		setMotorSyncEncoder(left, right, POWER_STRAIGHT, 100, SPEED_STRAIGHT);
		while (getMotorRunning(left) != 0)
			displayString(4, "Left encoder: %d", getMotorEncoder(left));
		float distanze = getUSDistance(sensorUltrasonic);
		if(distanze < 10 )
		{
			if(checkIsWall())
			{
				search = false;
			}
			else{
				search = false;
				raiseBalls();
			}
		}
	}
}

bool checkIsWall()
{
	setMotorSyncEncoder(left, right, POWER_TURN_LEFT, 100, SPEED_TURN);
	while (getMotorRunning(left) != 0)
		displayString(4, "Left encoder: %d", getMotorEncoder(left));
	sleep(50);
	float distance2 = getUSDistance(sensorUltrasonic);
	if(distance2 > 20)
		return true;
	return false;
}

void shiftArray(float* array, const UINT size)
{
		for(int i = size - 1; i > 0; i--)
		{
			array[i] = array[i-1];
		}
		array[0] = 0;
}

void calibrateGyro()
{
	// Switching the modes will cause the gyro sensor to recalibrate.
	getGyroRate(sensorGyro);

	resetGyro(sensorGyro);
	sleep(1000);

	getGyroDegrees(sensorGyro);

	resetGyro(sensorGyro);
	sleep(1000);

	lastDegrees = getGyroDegrees(sensorGyro);

	currentState = STATE_DEFAULT;
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

			case STATE_CALIBRATE_GYRO:
				displayTextLine(1, "STATE: Calibrate Gyro");
				calibrateGyro();
				break;

			case STATE_END:
				displayTextLine(1, "STATE: End");
				run = false;
				break;
		}
	}
}
