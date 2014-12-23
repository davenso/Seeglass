#include "stdafx.h"
#include "FeatExtractor.h"
#include <ctime>

using namespace std;
using namespace cv;

string detectionAlgo = "FAST";
string extractionAlgo = "FREAK";



FeatExtractor::FeatExtractor(string extractAlgorithm, string detectAlgorithm)
{
	extractionAlgo = extractAlgorithm;
	detectionAlgo = detectAlgorithm;
}

FeatExtractor::~FeatExtractor()
{

}


//Run Feature Extraction process
Mat FeatExtractor::Extract(Mat frame)
{
	//OPTIONALLY create copy of original frame
	Mat frame_orig = frame.clone();

	//preprocess frame
	//Mat frame_prepro = PreProcess(frame);

	//detect features
	vector<KeyPoint> keypoints = FeatDetection(frame);

	//extract features
	Mat descriptors = FeatExtraction(frame, keypoints);

	//return extracted features
	return descriptors;
}

Mat FeatExtractor::PreProcess(Mat frame)
{
	/* re-size */
	//int r = 100.0 / frame.shape[1];
	//int dim = (100, int(frame.shape[0] * r));

	////perform the actual resizing of the image and show it
	//cvResize(frame, frame, dim, INTER_AREA);


	/* Grayscale */
	Mat frame_gray;
	//frame.copyTo(frame_gray);
	//cvtColor(frame, frame_gray, CV_BGR2GRAY);

	return frame_gray;
}

vector<KeyPoint> FeatExtractor::FeatDetection(Mat frame)
{
	//create detector of type, [detectionAlgo]
	Ptr<FeatureDetector> detector = FastFeatureDetector::create(detectionAlgo);

	//create vector of keypoints for detected points
	vector<KeyPoint> keypoints;

	//run detector
		clock_t startTime_detect = clock();
	detector->detect(frame, keypoints);
		clock_t endTime_detect = clock();
		double timeSec_detect = (endTime_detect - startTime_detect) / static_cast<double>(CLOCKS_PER_SEC);
		cout << "Feature Detection time = " << timeSec_detect << "secs\n";

	return keypoints;
}

Mat FeatExtractor::FeatExtraction(Mat frame, vector<KeyPoint> keypoints)
{
	//create feature descriptor of type, [extractionAlgo]
	Ptr<DescriptorExtractor> descriptorsExtractor = DescriptorExtractor::create(extractionAlgo);

	//create Mat for extracted features
	Mat descriptors;

	//run descriptor
		clock_t startTime_extract = clock();
	descriptorsExtractor->compute(frame, keypoints, descriptors);
		clock_t endTime_extract = clock();
		double timeSec_extract = (endTime_extract - startTime_extract) / static_cast<double>(CLOCKS_PER_SEC);
		cout << "Feature Extraction time = " << timeSec_extract << "secs\n";

	return descriptors;
}