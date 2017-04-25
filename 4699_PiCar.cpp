/*
 * 4699_PiCar.cpp
 * 
 * Copyright 2017  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <time.h>
#include <unistd.h>
#include <iostream>

#include "Motor_Servo_Test.h"

#define maxSpeed 100
#define minSpeed 0

using namespace std;

int main(int argc, char **argv)
{
	char command;
	Motor_Servo motor;
	cout << endl << "******************************************\n\n";
	cout << "ELEX4699 Pi Car, by the Dream Team\n\n";
	cout << "******************************************\n\n";
	cout << "Press (I) for FWD\n\n";
	cout << "Press (K) for BWD\n\n";
	cout << "Press (J) for LEFT\n\n";
	cout << "Press (L) for RIGHT\n\n";
	cout << "Press (S) for STOP\n\n";
	cout << "Press (W) for SWIPE\n\n";
	cout << "Press (A) to increase speed\n\n";
	cout << "Press (D) to decrease speed\n\n";
	motor.servo(1200);
	
	int speed = 50;
	int direction = 1;
	int negspeed = (speed * -1);
	
	while (true)
	{
		cin >> command;
		int option = 0;

		if(command == 'i')
		{
			motor.Drive(speed, speed);
			direction = 1;	
			option = 1;
		}
		if(command == 'k')
		{
			negspeed = (speed * -1);
			motor.Drive(negspeed, negspeed);
			direction = 0;	
			option = 1;
		}
		if(command == 'j')
		{
			motor.Drive(0, speed);	
			option = 1;
		}
		if(command == 'l')
		{
			motor.Drive(speed, 0);	
			option = 1;
		}
		if(command == 's')
		{
			motor.shortBrake(true, true);
			option = 1;	
		}
		
		if(command == 'a')
		{
			if((speed > minSpeed)&& (speed < maxSpeed));
			{
				speed += 5;
			}
			
			if (direction == 0)
			{
				negspeed = (speed * -1);
				motor.Drive(negspeed, negspeed);
				
			}
			
			if (direction == 1)
			{
				
				motor.Drive(speed, speed);	
			}
			option = 1;
	
		}
		
			if(command == 'd')
		{
			if((speed > minSpeed) && (speed < maxSpeed))
			{
			speed -= 5;
			}
			
			if (direction == 0)
			{
				negspeed = (speed * -1);
				motor.Drive(negspeed, negspeed);	
			}
			
			if (direction == 1)
			{
				
				motor.Drive(speed, speed);	
			}
			
			option = 1;
		}
		
		if(command == 'w')
		{
			for (int sweep = 1200; sweep <= 2500; sweep++)
			{
				motor.servo(sweep);
				usleep(100);
			}
			motor.servo(1200);
			option = 1;
		
		}
		
		if(option == 0)
		{
			cout << "Not an invlaid input. Please try again" << endl;
		}
		
	}

	
	


	return 0;
}

