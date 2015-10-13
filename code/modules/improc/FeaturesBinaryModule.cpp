#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FeaturesBinaryModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void FeaturesBinaryModule::run( DataManager& data) const
{
  using namespace cv;

  // print params for debugging
  data.listParams();

  // get a pointer to the "image" input data
  Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
  // get the actual opencv matrix of the input data
  Mat img = oMatrix->getContent();

   Mat dist, rev = 255 - img;
  distanceTransform(rev, dist, CV_DIST_L2, 3);
  dist.convertTo(dist, CV_32FC1);
  
	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(dist));

}

// returns the meta data of this module
MetaData FeaturesBinaryModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {};

	return MetaData(
		"Show features of the image",
		"Image Processing",
		input,
		output,
		params
	);
}
