//#pragma once
#include "stdafx.h"
#include "Seeglass.h"

using namespace cv;					//opencv namespace

Seeglass seeglassModel;						//seeglass model
list<Mat> objects;					//list of objects for training model

void WelcomeMessage();
void AddObjectToList(Mat object);
void TestEM();
bool FrameExtract(string videoFilename);
Mat Segment(Mat image);

int main()		
{
	WelcomeMessage();

	seeglassModel = Seeglass();				//instantiate model
	objects = list<Mat>();			//create list of objects to hold raw training data

	/* KEY FRAME EXTRACTION  */
	string videoDir_prefix = "Data\\";
	string videoFileName = "wildlife.wmv";
	FrameExtract(videoDir_prefix + videoFileName);
	
	/* IMAGE SEGMENTATION */
	cout << "\n<Pipeline started>\n";
	string frameName = "wildlife_6.png";
	Mat temp = cv::imread(videoDir_prefix + frameName);		
	Mat object = Segment(temp);
	
	AddObjectToList(object);		//add object to list of items to train with

	/* CLASSIFICATION: Feature Extraction, Dimentionality Reduction and Feature Classification 
	These 3 core funtions are implemented in pipeline fashion within the class: Seeglass */	

	//Train seeglass model with list of objects
	for each (Mat object in objects)
	{
		//Train model with object
		//processing pipeline: (1). FeatureDetection (FAST/FREAK), (2). DimentionalityReduction (PCA), (3). Classification (EM)
		if (seeglassModel.Train(object))
			printf("\n<Seeglass model updated>.\n\n");
		else
			printf("\n<Model not updated>.\n\n");

		//pause for next data sample
		system("pause");
	}

	//save trained model to file
	seeglassModel.SaveModel("EM_model.yml");

	return 0;
}

bool FrameExtract(string videoFilename)
{
	FrameExtractor fe = FrameExtractor("HSV-Histogram");
	return fe.Extract(videoFilename);
}

Mat Segment(Mat image)
{
	Segmen segmen = Segmen("GrabCut");
	return segmen.Segment(image);
}

void TestEM()
{
	////sample of images
	//list<Mat> input_images;

	//Mat temp1 = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//input_images.push_back(temp1);
	//Mat temp2 = imread("ula.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//input_images.push_back(temp2);

	////feature dection, pca, train
	//model.Test_EM(input_images);
}

void WelcomeMessage()
{
	printf("--------------------------------------------------------------------------\n");
	printf("                              Seeglass                                    \n");
	printf("    Segmentation and Classification Framework for Video Intelligence      \n");
	printf("                       (c) David Ojika 2015                               \n");
	printf("--------------------------------------------------------------------------\n");
}

void AddObjectToList(Mat object)
{
	//convert to grayscale
	Mat object_gray;
	object.copyTo(object_gray);
	cvtColor(object, object_gray, CV_BGR2GRAY);

	//add to list 
	objects.push_back(object_gray);
}