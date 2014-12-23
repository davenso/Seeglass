#pragma once
#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FrameExtractor.h"
#include <ctime>
#include <string>

#define THRESH_MIN 0.2		//good for slow moving objects - get unique keyframes
#define THRESH_MAX 0.997	//good for fast moving objects


using namespace cv;
using namespace std;

bool HistogramMatch(Mat frame1, Mat frame2);
void KeyFrames_Save();

string algorthm = "HSV-Histogram";
int cameraIdx = 0;
vector<Mat> keyFrames;


FrameExtractor::FrameExtractor(string algorithm)
{
	algorthm = algorithm;
}

FrameExtractor::~FrameExtractor()
{

}

Mat frame_orig, frame;
bool FrameExtractor::Extract(string videoFilename)
{
	//create opencv capture object
	VideoCapture cap(videoFilename);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	//check if capture is open
	if (!cap.isOpened())
	{
		printf("Error: Cannot open video source.\n");
		return -1;
	}
	
	/* begin extracting frames */
	printf("\nExtracting key frames from video...");

	Mat frame1, frame2;
	Mat frame2_raw;
	Mat keyFrame;

	int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT) - 3;					/***WARNING: cant read last two frames!! ***/

	cap.read(frame1);		//printf("\nRead Frame%i\t", 1);				//read initial frame
	for (int i = 2; i <= frameCount; i++)									//read subsequent frames
	{
		cap.read(frame2);	//printf("\nRead Frame%i\t", i);					
		frame2.copyTo(frame2_raw);

		//use histogram algorithm for frame extraction
		if (!HistogramMatch(frame1, frame2))
		{
			//cout << to_string(i) << "\n";
			Mat temp;
			frame1.copyTo(temp);
			keyFrames.push_back(temp);										//is a keyframe
		}
		//prepare for next frame
		frame2_raw.copyTo(frame1);
		frame2.copyTo(frame1);

		//OPTIONALLY, show the video during capture process
		imshow("Input Video", frame2_raw);
		if (waitKey(1) >= 0) break;

//if (keyFrames.size() == 2) break;	//DEBUG CODE
	}
	printf(" detected: %i\n", keyFrames.size());

	KeyFrames_Save(videoFilename);

	return true;
}


void FrameExtractor::KeyFrames_Save(string videoFileName)
{
	Mat image;
	for (int i = 0; i < keyFrames.size(); i++)
	{
		string objectName = videoFileName.substr(0, videoFileName.length() - 4);
		imwrite(objectName + "_" + to_string(i + 1) + ".png", keyFrames[i]);
	}
}

bool FrameExtractor::HistogramMatch(Mat frame1, Mat frame2)
{
	//convert frames to HSV color space
	Mat frame1_hist, frame2_hist;

	cvtColor(frame1, frame1_hist, COLOR_BGR2HSV);
	cvtColor(frame2, frame2_hist, COLOR_BGR2HSV);

	//set histogram parameters
	int h_bins = 50; int s_bins = 60;
	int histSize [] = { h_bins, s_bins };

	float h_ranges [] = { 0, 180 };
	float s_ranges [] = { 0, 256 };

	const float* ranges [] = { h_ranges, s_ranges };
	int channels [] = { 0, 1 };								//Use the o-th and 1-st channels

	//calculate and normalize histograms
	MatND frame1_norm;
	MatND frame2_norm;

	calcHist(&frame1_hist, 1, channels, Mat(), frame1_norm, 2, histSize, ranges, true, false);
	normalize(frame1_norm, frame1_norm, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&frame2_hist, 1, channels, Mat(), frame2_norm, 2, histSize, ranges, true, false);
	normalize(frame2_norm, frame2_norm, 0, 1, NORM_MINMAX, -1, Mat());

	double result = compareHist(frame1_norm, frame2_norm, 0);		//Correlation comparion 

	float thresh_min = 0.2;		//get unique keyframes
	float thresh_max = 0.997;	//good for fast moving objects in video
	if (result < thresh_min)
		return false;			//not a match: likely different scenes

	return true;
}