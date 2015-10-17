#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "CartToPolarModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void cartToPolarModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// Declaring the variables to use for simple Edge detector
	int kernel_size = 3;
	int scale = 1;
  	int delta = 0;
	int dtype=-1;
	int boxSize=11;
	float eps = 0.001;
	int c,r;

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

	Mat grad_x, grad_y, grad_amp, grad_orient;

	int rows = m.rows;
  	int cols = m.cols;

  	Mat mean(rows, cols, CV_32FC1);
    Mat std(rows, cols, CV_32FC1);
    Mat pmr(rows, cols, CV_32FC1);
    Mat skw(rows, cols, CV_32FC1);
    Mat kur(rows, cols, CV_32FC1);
    Mat cnt(rows, cols, CV_32FC1);
    Mat hom(rows, cols, CV_32FC1);

    double cur_norm, cur_mean, cur_var, cur_pmr, cur_skw, cur_kur, cur_cnt, cur_hom;
    int new_x, new_y;

	//Sobel(InputArray src, OutputArray dst, int ddepth, int dx, int dy, int ksize=3, double scale=1,  double delta=0, int borderType=BORDER_DEFAULT );
  	Sobel(m, grad_x, CV_32FC1, 1, 0, kernel_size, scale, delta, BORDER_DEFAULT);
  	Sobel(m, grad_y, CV_32FC1, 0, 1, kernel_size, scale, delta, BORDER_DEFAULT);

	//cartToPolar(InputArray x, InputArray y, OutputArray magnitude, OutputArray angle, bool angleInDegrees=false)
  	cartToPolar(grad_x, grad_y, grad_amp, grad_orient, false);

  	Mat orient_mean, orient_norm;
 
  	// multiply(InputArray src1, InputArray src2, OutputArray dst, double scale=1, int dtype=-1 )
  	multiply(grad_amp, grad_orient, orient_mean, scale, dtype);
  
  	//blur(InputArray src, OutputArray dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT )
  	blur(orient_mean, orient_mean, Size(boxSize, boxSize), Point(-1,-1),BORDER_DEFAULT);
	threshold(orient_mean, orient_mean, 0, 0, CV_THRESH_TOZERO);

  	blur(grad_amp, orient_norm, Size(boxSize, boxSize));
  	threshold(orient_norm, orient_norm, 0, 0, CV_THRESH_TOZERO);

	// set the result (output) on the datamanager
	data.setOutputData("orient_mean",new Matrix(orient_mean));
	data.setOutputData("orient_norm",new Matrix(orient_norm));

}

// returns the meta data of this module
MetaData cartToPolarModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"orient_mean", DataDescription(MATRIX, "the result image.") },
		{"orient_norm", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
	};

	return MetaData(
		"Applies cartToPolar and blur to the image using openCV.  ",
		"Image Processing",
		input,
		output,
		params
	);
}
