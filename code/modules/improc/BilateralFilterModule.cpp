#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "BilateralFilterModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void BilateralFilterModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// Variables
	int MAX_KERNEL_LENGTH = 31;
	Mat dst;

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

	// Applying Bilateral Filter
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    { 
    	bilateralFilter ( m, dst, i, i*2, i/2 );
    }

	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(dst));

}

// returns the meta data of this module
MetaData BilateralFilterModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"MAX_KERNEL_LENGTH", ParamDescription("the max MAX_KERNEL_LENGTH for the given image", true) }
	};

	return MetaData(
		"Applies Bilateral Filtering to an image using openCV.",
		"Image Processing",
		input,
		output,
		params
	);
}
