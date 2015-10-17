#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GradientModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void GradientModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// Declaring the variables to use for simple Edge detector
	int scale = 1;
  	int delta = 0;
  	int ddepth = CV_16S;
  	
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

	//Declare the gradient image source
	Mat grad;

	/// Generate grad_x and grad_y
  	Mat grad_x, grad_y;
  	Mat abs_grad_x, abs_grad_y;

  	/// Gradient X
 	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
 	Sobel( m_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  	convertScaleAbs( grad_x, abs_grad_x );

  	/// Gradient Y
 	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
 	Sobel( m_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );

 	/// Total Gradient (approximate)
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(grad));

}

// returns the meta data of this module
MetaData GradientModule::getMetaData() const
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
		"Applies Gaussian blurring to an image using openCV.",
		"Image Processing",
		input,
		output,
		params
	);
}
