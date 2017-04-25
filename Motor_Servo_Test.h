#define AIN1 11
#define BIN1 6 
#define AIN2 9 
#define BIN2 13
#define PWMA 10
#define PWMB 19
#define STBY 5
#define PWMS 26

#pragma once

#include <iostream>
#include <pigpio.h>
#include <stdbool.h>

class Motor_Servo
{
	public:
	
	Motor_Servo();
	~Motor_Servo();

    //  General purpose drive function. Values for RIGHT and LEFT should be
    //  between -100 - 100. Positive numbers for fwd, negative for rev
    //  100 and -100 are full speed 
    void Drive(float RIGHT, float LEFT);

    //  standby differs from brake in that the outputs are hi-z instead of
    //  shorted together. After the constructor is called, the motors are in
    //  standby.
    void standby(bool disableMotors);

    //  shortBrake shorts the motor leads together to drag the motor to a halt
    //  as quickly as possible.
    void shortBrake(bool brakeRight, bool brakeLeft);

	void fwdA();
    void revA();
    void fwdB();
    void revB();
    
    void servo(int swipe);
    float _RIGHT, _LEFT;
};
