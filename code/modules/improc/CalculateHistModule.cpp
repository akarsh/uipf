#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CalculateHistModule.hpp"
#include <iostream>
#include <stdio.h>


using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void CalculateHistModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");

	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

  // Establish the number of bins
  int histSize = 256;

  // Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

   if (m.type()==CV_8UC1)
   {

    // Calculate histogram
    MatND hist;

    calcHist( &m, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false );

 
    // Plot the histogram
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
 
    Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
     
    for( int i = 1; i < histSize; i++ )
    {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
    }

  // set the result (output) on the datamanager 
  data.setOutputData("image",new Matrix(histImage));
  }
else
  {
    // Separate the image in 3 places ( B, G and R )
  	vector<Mat> bgr_planes;
  	split( m, bgr_planes );


  	bool uniform = true; bool accumulate = false;

  	Mat b_hist, g_hist, r_hist;

  	// Compute the histograms:
  	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  	// Draw the histograms for B, G and R
  	int hist_w = 512; int hist_h = 400;
  	int bin_w = cvRound( (double) hist_w/histSize );

  	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  	// Normalize the result to [ 0, histImage.rows ]
  	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  	// Draw for each channel
  	for( int i = 1; i < histSize; i++ )
  	{
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  	}

  // set the result (output) on the datamanager 
  data.setOutputData("image",new Matrix(histImage));
  }
 
}

// returns the meta data of this module
MetaData CalculateHistModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {};

	return MetaData(
		"Calculates Histogram of the input image",
		"Image Processing",
		input,
		output,
		params
	);
}
