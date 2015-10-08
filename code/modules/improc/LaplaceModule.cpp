#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "LaplaceModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void LaplaceModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// Declaring the variables to use for simple Edge detector
	int kernel_size = 3;
	int scale = 1;
  	int delta = 0;
  	int ddepth = CV_16S;
	int c;

	// read the params (window size and sigma) given for this step
	int nWindowSize = data.getParam<int>("windowSize", 0);
	double sigmaX = data.getParam<double>("sigmaX",0.0);
	double sigmaY = data.getParam<double>("sigmaY",0.0);

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

	// do gaussian blur using opencv
	GaussianBlur(m,m,Size( nWindowSize, nWindowSize ), sigmaX, sigmaY, BORDER_DEFAULT );

	// Declare the new source for the grayscale data
	Mat m_gray;

	// convert the filtered image to grayscale
	cvtColor(m,m_gray, CV_RGB2GRAY);

 	 // Apply Laplace function
 	 Mat abs_dst,dst;

 	 Laplacian( m_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
 	 convertScaleAbs( dst, abs_dst );

	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(abs_dst));

}

// returns the meta data of this module
MetaData LaplaceModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"windowSize", ParamDescription("window size of the kernel. Must be an odd number. Optional, defaults to 0 which means that it is calculated based on sigma.", true) },
		{"sigmaX", ParamDescription("variance of the gaussian kernel in X direction.") },
		{"sigmaY", ParamDescription("variance of the gaussian kernel in Y direction. Optional, defaults to sigmaX.", true) }
	};

	return MetaData(
		"Applies Gaussian blurring to an image, then Laplacian to show the silhouette in the result using openCV.  ",
		"Image Processing",
		input,
		output,
		params
	);
}
