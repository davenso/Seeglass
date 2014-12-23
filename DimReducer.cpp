#include "stdafx.h"
#include "DimReducer.h"
#include <ctime>

using namespace std;
using namespace cv;

string algo = "PCA";
int ncomponents = 0;		//defualt: retains all components

DimReducer::DimReducer(string algorithm, int numComps)
{
	algo = algorithm;
	ncomponents = numComps;
}

DimReducer::~DimReducer()
{

}

Mat DimReducer::PCA(Mat features)
{
	//mean vector: none Mat(), let PCA engine compute it
	cv::PCA pca = cv::PCA(features, Mat(), CV_PCA_DATA_AS_ROW, ncomponents);		//computed pca basis.

	return pca.project(features);		//projected data
}