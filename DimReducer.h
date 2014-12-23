#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class DimReducer
{
public:
	//constructor
	DimReducer(string algorithm, int numComps);
	~DimReducer();

	//methods
	Mat PCA(Mat features);

private:
	
};