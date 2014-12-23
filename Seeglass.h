#pragma once
#include "FrameExtractor.h"
#include "Segmen.h"
#include "FeatExtractor.h"
#include "DimReducer.h"
#include "Classifier.h"

#include <opencv2\opencv.hpp>


using namespace cv;
using namespace std;


class Seeglass
{
public:
	//constructor
	Seeglass();
	~Seeglass();

	//public methods	
	bool Train(Mat frame);		//Runs pipeline: FeatureExtraction, DimentionalityReduction, Classifcation
	void Test(Mat frame);	
	void SaveModel(string fileName);

	bool ExtractFrames(string videoFileName);
	void Test_EM(list<Mat> in_images);

private:
	//modules
	FeatExtractor featExtr;
	DimReducer dimReducer;
	Classifier classifier;

	//private methods
	Mat FeatureExtraction(Mat frame);
	Mat DimentionalityReduction(Mat features);
	bool Classification(Mat samples);
};