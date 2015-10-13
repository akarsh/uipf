#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FeaturesColorModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void FeaturesColorModule::run( DataManager& data) const
{
  using namespace cv;

  // print params for debugging
  data.listParams();

  // get a pointer to the "image" input data
  Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
  // get the actual opencv matrix of the input data
  Mat img = oMatrix->getContent();

  vector<Mat> channels;
  split(img, channels);

  Mat blue = channels.at(0)/255.;
  Mat green = channels.at(1)/255.;
  Mat red = channels.at(2)/255.;
  Mat blue2, green2, red2;
  Mat blue3,green3,red3;
  
  blue.convertTo(blue3, CV_32F);
  green.convertTo(green3,CV_32F);
  red.convertTo(red3, CV_32F);

  pow(blue3, 2, blue2);
  pow(green3, 2, green2);
  pow(red3, 2, red2);
  
  Mat sum = green2 + blue2 + red2;
  Mat sum2;
  sum.convertTo(sum2, CV_32F);
  
  pow(sum2, 0.5, sum);
  
	// set the result (output) on the datamanager
	data.setOutputData("image",new Matrix(sum));

}

// returns the meta data of this module
MetaData FeaturesColorModule::getMetaData() const
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
