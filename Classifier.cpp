
#include "stdafx.h"
#pragma once
#include "Classifier.h"
#include <ctime>



#include "FeatExtractor.h"
#include "DimReducer.h"


using namespace std;
using namespace cv;

string algori = "EM";

EM em_model;					//EM model
TermCriteria termCritaria;		//EM termination critaria

Mat asRowMatrix(const vector<Mat>& src, int rtype, double alpha, double beta);  /// remove
Mat FlattenMatrix(Mat img);

Classifier::Classifier(string algorithm)
{
	algori = algorithm;

	//define and crate ternmination critaria 
	int type = TermCriteria::COUNT + TermCriteria::EPS;
	int maxCount = 3; // EM::DEFAULT_MAX_ITERS;
	termCritaria = TermCriteria(type, maxCount, FLT_EPSILON);

	//define EM parameters and instantiate model
	int nclusters = EM::DEFAULT_NCLUSTERS;
	int covMatrix_type = EM::COV_MAT_DEFAULT;
	em_model = EM(nclusters, covMatrix_type, termCritaria);
}

Classifier::~Classifier()
{

}

int i = 1;
//Learn classifier
bool Classifier::Train(Mat sample)
{
	cout << "Training classfication model...";
		clock_t startTime_train = clock();
		em_model.train(sample);		
		clock_t endTime_train = clock();
		double timeSec_train = (endTime_train - startTime_train) / static_cast<double>(CLOCKS_PER_SEC);
		cout << " train time = " << timeSec_train << "secs\n";	

		SaveClassifierToFile("em" + to_string(i) + ".xml");
		i++;

	return em_model.isTrained();
}

void Classifier::SaveClassifierToFile(string fileName)
{
	if (em_model.isTrained() == false)
		return;

	FileStorage fs = FileStorage(fileName, FileStorage::WRITE);
	em_model.write(fs);

	fs.release();
}

void Prediction_test()
{
	/* Test prediction here */
	//sample.reshape(sample.cols);
	//Vec2d prediction = em_model.predict(sample, Mat());
}


void Classifier::Train_test(list<Mat> imgs)
{
	// Holds some features database
	vector<Mat> db;

	for each (Mat image in imgs)
	{
		//feature detection
		FeatExtractor fe = FeatExtractor("FREAK", "FAST");
		Mat features = fe.Extract(image);

		//pca
		DimReducer dr = DimReducer("PCA", 10);
		Mat samples = dr.PCA(features);

		//add to database as row matrix
		db.push_back(samples);
	}

	/* train with database */

	//define and crate ternmination critaria 
	int _type = TermCriteria::COUNT + TermCriteria::EPS;
	int _maxCount = 3; // EM::DEFAULT_MAX_ITERS;
	TermCriteria _termCritaria = TermCriteria(_type, _maxCount, FLT_EPSILON);

	//define EM parameters and instantiate model
	int _nclusters = EM::DEFAULT_NCLUSTERS;
	int _covMatrix_type = EM::COV_MAT_DEFAULT;
	EM em_classfr = EM(_nclusters, _covMatrix_type, _termCritaria);


	// Build a matrix with the observations in row:
	Mat dat = asRowMatrix(db, CV_32FC1, 1, 0);
	em_classfr.train(db);
}


Mat FlattenMatrix(Mat img)
{
	int rtype = CV_32FC1; double alpha = 1; double beta = 0;

	Mat samples(img.rows * img.cols, 3, CV_32FC1);

	int index = 0;
	for (int y = 0; y < img.rows; y++) 
	{
		if (img.isContinuous()) {
			img.reshape(1, 1).convertTo(img.row(y), rtype, alpha, beta);
		}
		else {
			img.clone().reshape(1, 1).convertTo(img.row(y), rtype, alpha, beta);
		}
	}
	return samples;
}


// Converts the images given in src into a row matrix.
Mat asRowMatrix(const vector<Mat>& src, int rtype, double alpha = 1, double beta = 0) 
{
	 //Number of samples:
	size_t n = src.size();

	 //Return empty matrix if no matrices given:
	if (n == 0)
		return Mat();

	 //dimensionality of (reshaped) samples
	size_t d = src[0].total();

	 //Create resulting data matrix:
	Mat data(n, d, rtype);

	 //Now copy data:
	for (int i = 0; i < n; i++) {
		
		if (src[i].empty()) {
			string error_message = format("Image number %d was empty, please check your input data.", i);
			CV_Error(CV_StsBadArg, error_message);
		}
		 //Make sure data can be reshaped, throw a meaningful exception if not!
		if (src[i].total() != d) {
			string error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, src[i].total());
			CV_Error(CV_StsBadArg, error_message);
		}
		 //Get a hold of the current row:
		Mat xi = data.row(i);
		 //Make reshape happy by cloning for non-continuous matrices:
		if (src[i].isContinuous()) {
			src[i].reshape(1, 1).convertTo(xi, rtype, alpha, beta);
		}
		else {
			src[i].clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
		}
	}
	return data;
}


