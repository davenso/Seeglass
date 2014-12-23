#include "stdafx.h"
#include <ctime>
#pragma once
#include "Segmen.h"

using namespace std;
using namespace cv;

string alg = "GrabCut";

Segmen::Segmen(string algorithm)
{
	alg = algorithm;
}

Segmen::~Segmen()
{
}

Mat Segmen::Segment(Mat image)
{
	//everything outside this box will be set to def. 
	//background GC_BGD, clearly from the image you can see that the players legs are outside the box, 
	//so this will cause problems. you need to either change the box, 
	//such that everything is outside the box is the background, or use your mask to scribble on the players legs in green.  
	//cv::Rect rectangle(50, 50, 450, 290);		// startX, startY, width, height);
	//cv::Mat bgModel, fgModel;

	////find the mask
	//Mat mask;
	//mask.create(image.size(), CV_8UC1);  //CV_8UC1 is single channel
	//mask.setTo(Scalar::all(GC_BGD));  //you have set it to all def. background 

	//Mat maskImg;
	//image.copyTo(maskImg);


	//for (int i = 0; i<maskImg.cols; i++)
	//	for (int j = 0; j<maskImg.rows; j++)
	//	{
	//	//if it's red, make it black
	//	if ((int) maskImg.at<cv::Vec3b>(j, i)[0] == 0 && (int) maskImg.at<cv::Vec3b>(j, i)[1] == 0 && (int) maskImg.at<cv::Vec3b>(j, i)[2] == 255) {

	//		//the whole mask is black so this is redundant
	//		mask.at<uchar>(j, i) = GC_BGD;  //GC_BGD := 0 := black 

	//	}

	//	//if it's green, make it white
	//	if ((int) maskImg.at<cv::Vec3b>(j, i)[0] == 0 && (int) maskImg.at<cv::Vec3b>(j, i)[1] == 255 && (int) maskImg.at<cv::Vec3b>(j, i)[2] == 0) {

	//		mask.at<uchar>(j, i) = GC_FGD; //GC_FGD:= 1 := white 
	//	}
	//}




	// define bounding rectangle 
	//cv::Rect rectangle(50, 70, image.cols - 150, image.rows - 180);
	
	cv::Rect rectangle(7, 121, 779, 701);

	Mat result;				// segmentation result (4 possible values)
	Mat bgModel, fgModel;	// the models (internally used)
	
	// GrabCut segmentation
			clock_t startTime_segment = clock();
			//grabCut(image, mask, rectangle, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
	grabCut(image,			// input image
		result,					// segmentation result
		rectangle,				// rectangle containing foreground 
		bgModel, fgModel,		// models
		1,						// number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle
			clock_t endTime_segment = clock();
			double timeSec_segment = (endTime_segment - startTime_segment) / static_cast<double>(CLOCKS_PER_SEC);
			cout << "\nSegmentation time = " << timeSec_segment << "secs\n";

	// Get the pixels marked as likely foreground
	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);

	// Generate output image
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(foreground, result); // bg pixels not copied

	// draw rectangle on original image
	//cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);
	//cv::namedWindow("Image");
	//cv::imshow("Image", image);

	//// display result
	//cv::namedWindow("Segmented Image");
	//cv::imshow("Segmented Image", foreground);

	return foreground;
}