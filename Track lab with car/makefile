# The compiler to use is for C++
CC=g++
# The compiler options are (all warnings)
CFLAGS=-Wall `pkg-config --cflags opencv` -std=c++11 -c
# The linker options are (all warnings)
LDFLAGS=-Wall `pkg-config --libs opencv` -std=c++11 -lpigpio -lrt -lpthread -lraspicam -lraspicam_cv 


all: Track

Track: 4618_Lab7.o Track.o Motor_Servo_Test.o 
	$(CC) $(LDFLAGS) 4618_Lab7.o Track.o Motor_Servo_Test.o -o Track

4618_Lab7.o: 4618_Lab7.cpp
	$(CC) $(CFLAGS) 4618_Lab7.cpp
	
Track.o: Track.cpp
	$(CC) $(CFLAGS) Track.cpp	

Motor_Servo_Test.o: Motor_Servo_Test.cpp
	$(CC) $(CFLAGS) Motor_Servo_Test.cpp
	
clean:
	rm -f *o Track





