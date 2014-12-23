#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class FrameExtractor
{
public:
	//constructor
	FrameExtractor(string algorithm);
	~FrameExtractor();

	//methods
	bool Extract(string videoFilename);

private:
	bool HistogramMatch(Mat frame1, Mat frame2);
	void KeyFrames_Save(string videoFileName);
};