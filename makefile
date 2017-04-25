# The compiler to use is for C++
CC=g++
# The compiler options are (all warnings)
CFLAGS=-Wall `pkg-config --cflags opencv` -std=c++11 -c
# The linker options are (all warnings)
LDFLAGS=-Wall `pkg-config --libs opencv` -std=c++11 -lpigpio -lrt -lpthread -lraspicam -lraspicam_cv 


all: Motor

Motor: 4699_PiCar.o Motor_Servo_Test.o 
	$(CC) $(LDFLAGS) 4699_PiCar.o Motor_Servo_Test.o -o Motor

4699_PiCar.o: 4699_PiCar.cpp
	$(CC) $(CFLAGS) 4699_PiCar.cpp
	
Motor_Servo_Test.o: Motor_Servo_Test.cpp
	$(CC) $(CFLAGS) Motor_Servo_Test.cpp	

clean:
	rm -f *o Motor





