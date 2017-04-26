////////////////////////////////////////////////////////////////
// ELEX 4618 Lab 6
// Created Oct 5, 2016 by Chris Stewart
////////////////////////////////////////////////////////////////
//#include "stdafx.h" // comment out for pi
#include <iostream>
#include "Track.h"

#define PI4618
//#define WIN4618

#ifdef WIN4618
#include "opencv.hpp"
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")
#endif

#ifdef PI4816
#include <opencv2/opencv.hpp>
#endif

   
int main(int argc, char **argv)
{
		CTrack track;
		track.Run();
}
