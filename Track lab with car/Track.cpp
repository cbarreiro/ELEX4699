//#include "stdafx.h" // comment out for pi
#include "Track.h"
#include "iostream"
#define PI4618
//#WIN4618

using namespace cv;
using namespace std;

CTrack::CTrack()
{
	#ifdef PI4618
	//Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
	Camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);	// Camera Width
	Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);	// Camera Height
	#endif
	

	
	#ifdef PI4618
	while( !Camera.open())

	{
		cout << "No Camera Detected";
	 }
	 
	
	waitKey(1000);
	

	Camera.grab();
	Camera.retrieve(imgTmp);
	#endif
	
  
	
	
	unedited_image = Mat::zeros(imgTmp.size(), CV_8UC3);
	segmented_image = Mat::zeros(imgTmp.size(), CV_8UC3);
	FinalImage = Mat::zeros(imgTmp.size(), CV_8UC3);
	DrawImage = Mat::zeros(imgTmp.size(), CV_8UC3);

	// Default HSV values for green sticker detection
	ColourChoice = 0;
	
	// Default pwm speeds for motor
	pwmL = 30;
	pwmR = 30;

	//	variables of Previous Camera Location
	lastX = -1; //last x value for object being tracked
	lastY = -1; //last y value for object being tracked
	_thread_exit = false; // threads always loop, change to be used to exit threads 	
}

CTrack::~CTrack()
{
	 motor.standby(true);
	 Camera.release();
}



void CTrack::Update()
{
	// threading
	auto start_time2 = chrono::steady_clock::now();
	auto end_time2 = start_time2 + frame_duration2(1);

	// Default Values For Contours
	largest_contour = -1;
	largest_area = 0;


	
#ifdef PI4618
	Camera.grab();
	Camera.retrieve(unedited_image);
#endif
	
	//Convert unedited webcam image from BGR to HSV and store as imgHSV
	cvtColor(unedited_image, imgHSV, COLOR_BGR2HSV); 
	
	// Controled with Menu Bar
	Colour(); // Select Colour to track
	//segmentation of imgHSV to slect colour
	inRange(imgHSV, Scalar(lowHue, iLowS, lowValue), Scalar(highHue, highSat, highValue), mask1); 
	inRange(imgHSV, Scalar(redlowHue, iLowS, lowValue), Scalar(redhighHue, highSat, highValue), mask2);
	segmented_image = mask1 | mask2;	// used to cover both ends of HSV Spectrum to track red objects
	
	//Erosion then Dilation = 
	erode(segmented_image, segmented_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(segmented_image, segmented_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//Dilation then Erosion = Closing - remove small holes from the foreground
	dilate(segmented_image, segmented_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(segmented_image, segmented_image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//	Test Mode
	if (mode == 2)
	{
		
		//// find contours
		findContours(segmented_image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		//  Calcualte largest contour and store in largest contour vector
		for (unsigned int i = 0; i < contours.size(); i++) // cycle through all contours
		{
			double area = contourArea(contours[i], false); // find area of current contour
			if (area > largest_area)	// if area is larger then previous area
			{
				largest_area = area;	// set as mew largest area
				largest_contour = i;	// idex for largest contour
				//bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
				minEnclosingCircle(contours[i], center, radius);	// Find minimum enlcosing circle of largest contour
			}

		}
		if (largest_contour >= 0)
		{
			//drawContours(unedited_image, contours, -1, Scalar(0, 0, 255), -1, 8); // Fill largest contour, Exact size
			//rectangle(unedited_image, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);	// Add rectamgle Selecting largest contour
			circle(unedited_image, center, (int)radius, Scalar(0, 0, 255), 1, 8, 0); // Filled circle to match contour
			circle(unedited_image, Point(320,240), (int)(radius / 2), Scalar(0, 0, 255), 1, 8, 0); 
		}
		trackmutex.lock();
		unedited_image.copyTo(FinalImage);
		trackmutex.unlock();
	}
	if (mode == 2)
	{
		//float f = ((radius * 2 * 50.0) / 4.0);


#ifdef PI4618
		trackmutex.lock();
		 distance = 4.0 * 514.195 / (2 * radius);
		 trackmutex.unlock();
#endif
		 
		objectDistance = "Object is: " + to_string(distance) + " cm away";
		//cout << f << endl;
		
		/////////////////////////////////////////////////////////////////
		// Calculate moments to find Center Of Largest Contour
		/////////////////////////////////////////////////////////////////
		//Image_moment = moments(segmented_image);
		//M01 = Image_moment.m01;
		//M10 = Image_moment.m10;
		//Area = Image_moment.m00;
	
		//if (Area > 100)	// test for noise (Comment out To Use Bounding Center)
		//{
			posX = M10 / Area;
			posY = M01 / Area;
		
		// calculate the position of the ball using Bounding Center 
		posX = center.x;
		posY = center.y;
		
		float minX = 320 - radius;
		
		float maxX = 320 + radius;
		
		float fminX = 320 - (radius + 80);
		
		float fmaxX = 320 + (radius + 80);
		
		//cout << minX << " " << fminX << endl;
		
		
			
			if (lastX >= 0 && posX >= 0)
			{


#ifdef PI4618
				int slow = 1;
				
				int fastx = 1.5;
				
				
				

#endif
				// Test to See if Object is in Left Field of View
				if (posX < minX)	
				{
					pwmR -= slow;
					pwmL += slow;	// IF almost Center slow down to 1/ Cycle
					if (posX < fminX)
					{
						
						pwmR -= fastx;
						pwmL += fastx;	// If Camera is far from centered shift 2/ cycle
						
					}
					cout << "left" << endl;

				}
				
				

				// Test to See if Object is in Right Field of View
				if (posX > maxX)	
				{
					pwmR += slow;
					pwmL -= slow;	// If Camera is far from centered shift 1/ cycle
					if (posX > fmaxX)
					{
						pwmL -= fastx;	// If Camera is far from centered shift 2/ cycle
						pwmR += fastx;
						
					}
				cout << "right" << endl;
				}
				
				

				if (pwmL < minSpeed)
						{
							pwmL = minSpeed;
						}
						
				if (pwmL > maxSpeed)
						{
							pwmL = maxSpeed;
						}
				
				if (pwmR < minSpeed)
						{
							pwmR = minSpeed;
						}
						
				if (pwmR > maxSpeed)
						{
							pwmR = maxSpeed;
						}


				motor.Drive(pwmR , pwmL);


			 }
			lastX = posX;
			
		} 
	//}	// Comment Out to Use Bounding Center
	this_thread::sleep_until(end_time2);
}

void CTrack::Draw()
 {
	auto start_time = chrono::steady_clock::now();
	auto end_time = start_time + frame_duration1(1);


	// test FPS
	freq = getTickFrequency();
	string elapsedtime;
	stop_tic = getTickCount();
	elapsed_time = freq / (stop_tic - start_tic);
	elapsedtime = "FPS " + to_string(elapsed_time);
	start_tic = getTickCount();
	
	// Calibration Mode
	if (mode == 1)
	{
		//	Calibration Window called Menu
		namedWindow("Menu", CV_WINDOW_NORMAL); 
		
		createTrackbar("Low Hue", "Menu", &lowHue, 179); //Hue Range (0 - 179)
		createTrackbar("High Hue", "Menu", &highHue, 179);

		createTrackbar("Low redHue", "Menu", &redlowHue, 179); //Hue Range for red wrap(0 - 179)
		createTrackbar("High redHue", "Menu", &redhighHue, 179);

		createTrackbar("Low Sat", "Menu", &iLowS, 255); //Saturation Ragne (0 - 255)
		createTrackbar("High Sat", "Menu", &highSat, 255);

		createTrackbar("Low Value", "Menu", &lowValue, 255);//Value Range (0 - 255)
		createTrackbar("High Value", "Menu", &highValue, 255);
	}


	trackmutex.lock();
	FinalImage.copyTo(DrawImage);
	trackmutex.unlock();

	putText(DrawImage, elapsedtime, Point(420, 460), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 4);
	putText(DrawImage, objectDistance, Point(15, 30), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 4);
	
	//	Test Mode
	if (mode == 2)
	{
		/*FinalImage = tracked_object | unedited_image;
		imshow("Object Tracked", FinalImage);*/
		//imshow("FPS", tracked_object);
		imshow("Draw Image", DrawImage); //show the Un-edited image
	}

	//	 Callibration Mode
	if (mode == 1)
	{
		trackmutex.lock();
		imshow("Segmented Image", segmented_image);
		trackmutex.unlock();
	}
	
	waitKey(1);
	
	this_thread::sleep_until(end_time);
}

void CTrack::Run()
{
	int reset = 1;
	cout << endl << "******************************************\n\n";
	cout << "ELEX4618 Object Tracker, by Chris Barreiro Stewart\n\n";
	cout << "******************************************\n\n";
	while (reset == 1)
	{
		// User Menu
		
		cout << "Press (1) for Calibration Mode\n\n";
		cout << "Press (2) for Test Mode\n\n";
		cin >> mode;
		reset = 0;

		if (mode != 2)
		{
			if (mode != 1)
			{
				cout << "Please Try again. Only select option 1 or 2" << endl;
				reset = 1;
			}
		}
	}
	// To Remove Threading
	/*while (1)
	{
		Update();
		Draw();
	}*/

	// Threading 
	thread t1(&CTrack::Update_thread, this);
	thread t2(&CTrack::Draw_thread, this);
	thread t3(&CTrack::Drive_thread, this);
	t1.join();
	t2.join();
	t3.detach();
	
}

void CTrack::Update_thread(CTrack* ptr)
{
	
	while (ptr->_thread_exit == false)
	{
		ptr->Update();
	}
	
}

void CTrack::Draw_thread(CTrack* ptr)
{
	
	while (ptr->_thread_exit == false)
	{
		ptr->Draw();
	}
	
}

void CTrack::Drive_thread(CTrack* ptr)
{
	
	while (ptr->_thread_exit == false)
	{
		ptr->carDrive();
	}
	
}

void CTrack::carDrive()
{
	// threading
	//auto start_time3 = chrono::steady_clock::now();
	//auto end_time3 = start_time3 + frame_duration3(1);
	trackmutex.lock();
	float straight = distance;
	float L = pwmL;
	float R = pwmR;
	trackmutex.unlock();
	
	if(straight > 12)
	{
		//motor.Drive(R, L);
	}
	else
	{
		motor.shortBrake(true, true);
	}
	//	this_thread::sleep_until(end_time3);
}

void CTrack::Colour()
{
	if (mode == 2)
	{
		if (ColourChoice == 0)
		{
			// Default HSV values for green sticker detection
			lowHue = 28;
			highHue = 74;
			redlowHue = 0;
			redhighHue = 0;
			iLowS = 70;
			highSat = 255;
			lowValue = 57;
			highValue = 223;


		}
		if (ColourChoice == 1)
		{
			// Default HSV values for Green detection
			lowHue = 58;
			highHue = 91;
			redlowHue = 0;
			redhighHue = 0;
			iLowS = 119;
			highSat = 254;
			lowValue = 49;
			highValue = 124;


		}
		if (ColourChoice == 2) // orange
		{
			// Default HSV values for Orange detection
			lowHue = 0;
			highHue = 13;
			redlowHue = 0;
			redhighHue = 0;
			iLowS = 158;
			highSat = 250;
			lowValue = 55;
			highValue = 176;

		}
	}
}
