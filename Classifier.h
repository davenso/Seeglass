#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Classifier
{
public:
	//constructor
	Classifier(string algorithm);
	~Classifier();

	//methods
	bool Train(Mat frame);
	void SaveClassifierToFile(string fileName);
	
	void Classifier::Train_test(list<Mat> imgs);
private:
	
};