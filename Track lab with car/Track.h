#pragma once

// #include "stafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#define PI4618
//#define WIN4618




#ifdef WIN4618
#include "opencv.hpp"
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")
#include "Control.h"
#define MAXTILT 170
#define MINTILT 30
#define MAXPAN 150
#define MINPAN 60
#endif

#ifdef PI4618
#include "Motor_Servo_Test.h"
#include <ctime>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#define maxSpeed 40
#define minSpeed 20

using namespace raspicam;
#endif


using namespace cv;
using namespace std;


/**
*
* @brief Implements start, ball, update, and draw methods.
*
* This class simulates play of the game Track.
*
* @author Chris Barreiro Stewart
*
*
*/
class CTrack 
{

private:
	
	Motor_Servo motor;
	int lowHue;
	int highHue;
	int redlowHue;
	int redhighHue;
	int iLowS;
	int highSat;
	int lowValue;
	int highValue;
	int mode;
	int ColourChoice;
	float distance;
	
	string objectDistance;

	float pwmL, pwmR;

	int lastX;
	int lastY;

	Moments Image_moment;
	double M01;
	double M10;
	double Area;
	recursive_mutex trackmutex;
	float posX;
	float posY;
	
	#ifdef PI4618
	RaspiCam_Cv Camera;
	
	#endif
	
	#ifdef WIN4618
	VideoCapture cap; // capture video from webcam channel 0
	#endif
	
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours; // vector for storing all contours
	int largest_contour;	// vector storing largest contour
	
	int largest_area;	// variable for largest area
	Rect bounding_rect;	// rectangle to outline largest contour
	Point2f center;
	float radius;

	

	bool _thread_exit; ///< Variable that exits threads
	double start_tic; ///< Variable set to start time of Draw method
	double stop_tic; ///< Variable set to stop time of Draw method
	double freq; ///< Variable set to frequency
	double elapsed_time; ///< Variable set to elapsed time


						 /** @brief Update thread
						 *
						 * @param void
						 * @return void
						 */
	static void Update_thread(CTrack* ptr);

	/** @brief Draw thread
	*
	* @param void
	* @return void
	*/
	static void Draw_thread(CTrack* ptr);
	
	static void Drive_thread(CTrack* ptr);

	typedef chrono::duration<int, ratio<1, 30>> frame_duration1;
	typedef chrono::duration<int, ratio<1, 30>> frame_duration2;
	typedef chrono::duration<int, ratio<1, 30>> frame_duration3;
	

public:
	CTrack();
	~CTrack();

	/** @brief Updates game data
	*
	* @param void
	* @return void
	*/
	void Update();

	/** @brief Draws game data
	*
	* @param void
	* @return void
	*/
	void Draw();

	/** @brief Runs game
	*
	* @param void
	* @return void
	*/
	void Run();

	void Colour();

	void carDrive();


	Mat unedited_image;
	Mat imgHSV, imgfilter;	 ///< image converted from RGB to HSV
	Mat imgTmp;
	Mat segmented_image, mask1, mask2; ///< segmented image
	Mat FinalImage;
	Mat DrawImage;




};
