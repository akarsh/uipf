#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "SFDetModule.hpp"
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace uipf;


// runs the module
/*
data	DataManager handles the input and ouput of this module
*/
void SFDetModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// read the params (minHessian) given for this step
	int minHessian = data.getParam<int>("minHessian", 400);

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat m_1 = oMatrix->getContent();

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix_1 = data.getInputData<Matrix>("image_2");
	// get the actual opencv matrix of the input data
	Mat m_2 = oMatrix_1->getContent();

	if (!m_1.data || !m_2.data)// Check for invalid input
	{
	 throw ErrorException(string("Error reading the images"));
	}

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( m_1, keypoints_1 );
  detector.detect( m_2, keypoints_2 );

  //-- Draw keypoints
  Mat m_keypoints_1; Mat m_keypoints_2;

  drawKeypoints( m_1, keypoints_1, m_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  drawKeypoints( m_2, keypoints_2, m_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );



	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(m_keypoints_1));
	data.setOutputData("image_2",new Matrix(m_keypoints_2));

}

// returns the meta data of this module
MetaData SFDetModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"minHessian", ParamDescription("minHessian is a threshold to decide from which value you are willing to accept keypoints.", true) }
		};

	return MetaData(
		"Detect the keypoints using SURF Detector from an image using openCV.",
		"Dummy",
		input,
		output,
		params
	);
}

