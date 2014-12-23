#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class FeatExtractor
{
public:
	//constructor
	FeatExtractor(string extractAlgorithm, string detectAlgorithm);
	~FeatExtractor();

	//methods
	Mat Extract(Mat frame);

private:
	Mat PreProcess(Mat frame);
	vector<KeyPoint> FeatDetection(Mat frame);
	Mat FeatExtraction(Mat frame, vector<KeyPoint> keypoints);
};