/*
 * Motor_Servo_Test.cpp
 * 
 * Author: Chris Barreiro Stewart
 * 
 */


#include "Motor_Servo_Test.h"

	Motor_Servo::Motor_Servo()
	{
		gpioInitialise();
		
	
		// Set Pins as Outputs
		gpioSetMode(AIN1, PI_OUTPUT); // Setup AIN1 as Output
		gpioSetMode(BIN1, PI_OUTPUT); // Setup BIN1 as Output
		gpioSetMode(AIN2, PI_OUTPUT); // Setup AIN2 as Output
		gpioSetMode(BIN2, PI_OUTPUT); // Setup BIN2 as Output
		gpioSetMode(PWMA, PI_OUTPUT); // Setup PWMA as Output
		gpioSetMode(PWMB, PI_OUTPUT); // Setup PWMB as Output
		gpioSetMode(STBY, PI_OUTPUT); // Setup PWMS as Output
		gpioSetMode(PWMS, PI_OUTPUT); // Setup PWMS as Output
		
		gpioWrite(STBY, 1); // Set STBY HIGH
		
		// Set PWM RANGE
		gpioSetPWMrange(PWMA, 100);
		gpioSetPWMrange(PWMB, 100);
		
	}
	
	Motor_Servo::~Motor_Servo()
	{
		gpioTerminate();
	}

void Motor_Servo::Drive(float RIGHT, float LEFT)
{
	// Store Values For Future Use
  _RIGHT=RIGHT;
  _LEFT=LEFT;
  
  //
  if (RIGHT < 0)
  {
    revA();
    RIGHT *= -1;
  }
  else
  {
    fwdA();
  }
  if (LEFT < 0)
  {
    revB();
    LEFT *= -1;
  }
  else
  {
    fwdB();
  }
 gpioPWM(PWMA, RIGHT);
 gpioPWM(PWMB, LEFT);
  
}

void Motor_Servo::standby(bool disableMotors)
{
  if (disableMotors)
  {
	gpioWrite(STBY, 0); // Set STBY LOW
  }
  else
  {
	gpioWrite(STBY, 1); // Set BIN1 HIGH
  }
}

void Motor_Servo::shortBrake(bool brakeRight, bool brakeLeft)
{
  if (brakeRight)
  {
	gpioWrite(AIN1, 1); // Set AIN1 HIGH
	gpioWrite(AIN1, 1); // Set AIN1 HIGH
 
  }
  if (brakeLeft)
  {
	gpioWrite(BIN1, 1); // Set BIN1 HIGH
	gpioWrite(BIN1, 1); // Set BIN1 HIGH
  }
}


void Motor_Servo::fwdA()
{
	gpioWrite(AIN1, 0); // Set AIN1 LOW
	gpioWrite(AIN2, 1); // Set AIN2 HIGH

}

void Motor_Servo::revA()
{
	gpioWrite(AIN1, 1); // Set AIN1 HIGH
	gpioWrite(AIN2, 0); // Set AIN2 LOW
}

void Motor_Servo::fwdB()
{
	gpioWrite(BIN1, 0); // Set BIN1 LOW
	gpioWrite(BIN2, 1); // Set BIN2 HIGH
}

void Motor_Servo::revB()
{
	gpioWrite(BIN1, 1); // Set BIN1 HIGH
	gpioWrite(BIN2, 0); // Set BIN2 LOW
}

void Motor_Servo::servo(int swipe)
{
	gpioServo(PWMS, swipe);
	gpioDelay(1000);
}
