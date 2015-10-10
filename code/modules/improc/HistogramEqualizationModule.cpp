#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "HistogramEqualizationModule.hpp"
#include <iostream>
#include <stdio.h>


using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void HistogramEqualizationModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");

	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

	//define new variable for the output data
 	Mat m_new; 

 	if (m.type()==CV_8UC1)
  	  {
 	    // Apply Histogram Equalization
	    equalizeHist( m, m_new );
	  }
	  else
	  {
	   vector<Mat> channels; 

           cvtColor(m, m_new, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format

           split(m_new,channels); //split the image into channels

           equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)

   	   merge(channels,m_new); //merge 3 channels including the modified 1st channel into one image

           cvtColor(m_new, m_new, CV_YCrCb2BGR); //change the color image from YCrCb to BGR format (to display image properly)
	  }

	// set the result (output) on the datamanager 
	data.setOutputData("image",new Matrix(m_new));

}

// returns the meta data of this module
MetaData HistogramEqualizationModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {};

	return MetaData(
		"Applies Histogram Equalization to the input image",
		"Image Processing",
		input,
		output,
		params
	);
}
