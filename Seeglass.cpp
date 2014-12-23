#include "stdafx.h"
#pragma once
#include "Seeglass.h"			//main system header

using namespace cv;				//opencv namespace

//system parameters
struct SeeglassParams
{
	string FeatDetect_Algo   = "FAST";
	string FeatExtract_Algo  = "FREAK";
	string DimReduce_Algo    = "PCA";
	int    DimReduce_nComps  = 11;
	string Classifi_Algo     = "EM";
} seeglass_params;



//constructor
Seeglass::Seeglass()

	/* instantiate modules */
	: featExtr(seeglass_params.FeatExtract_Algo, seeglass_params.FeatDetect_Algo),
	  dimReducer(seeglass_params.DimReduce_Algo, seeglass_params.DimReduce_nComps),
	  classifier(seeglass_params.Classifi_Algo)
{

}


//destructor
Seeglass::~Seeglass()
{

}


//
void Seeglass::Test_EM(list<Mat> in_images)
{
	classifier.Train_test(in_images);
}

bool ExtractFrames(string videoFileName)
{
	FrameExtractor frameExtractor = FrameExtractor("HSV_Histogram");
	return frameExtractor.Extract(videoFileName);
}
//

//Main function: Train
bool Seeglass::Train(Mat object)
{
	/* Feature Extraction */
	Mat features = FeatureExtraction(object);

	/* Dimentionality Reduction */
	Mat samples = DimentionalityReduction(features);

	/* Feature Classifciation */
	return Classification(samples);
}


/*... HELPER FUNCTIONS ...*/

//Module 1: Feature Extraction
Mat Seeglass::FeatureExtraction(Mat frame)
{	
	return featExtr.Extract(frame);				
}

//Module 2: Dimentionality Reduction
Mat Seeglass::DimentionalityReduction(Mat features)
{
	return dimReducer.PCA(features);
}

//Module 3: Classfication
bool Seeglass::Classification(Mat samples)
{
	return classifier.Train(samples);							
}

void Seeglass::SaveModel(string fileName)
{
	classifier.SaveClassifierToFile(fileName);
}