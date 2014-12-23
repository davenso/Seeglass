#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Segmen
{
public:
	//constructor
	Segmen(string algorithm);
	~Segmen();

	//methods
	Mat Segment(Mat image);
private:

};