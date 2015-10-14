#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "LoadImageTwoModule.hpp"
#include "../framework/types/Matrix.hpp"

using namespace std;
using namespace uipf;


/*

*/
void LoadImageTwoModule::run( DataManager& data) const
{
	 using namespace cv;

	  Mat image_2;
	  std::string strFilename = data.getParam<std::string>("filename","");
	  // check whether to load the image in grayscale mode, defaults to color
	  if (data.getParam<std::string>("mode","color").compare("grayscale") == 0)
	  {
	    image_2 = imread (strFilename.c_str (), CV_LOAD_IMAGE_GRAYSCALE);	// Read the file
	  }
	  else
	  {
	    image_2 = imread (strFilename.c_str (), CV_LOAD_IMAGE_COLOR);	// Read the file
	  }

	  if (!image_2.data)// Check for invalid input
	  {
	    throw ErrorException(string("Could not open or find the image: ") + strFilename);
	  }

	  data.setOutputData("image_2",new Matrix(image_2));

}

MetaData LoadImageTwoModule::getMetaData() const
{
	map<string, DataDescription> output = {
		{"image_2", DataDescription(MATRIX, "the loaded image.") }
	};
	map<string, ParamDescription> params = {
		{"filename", ParamDescription("file name of the file to load from.") },
		{"mode", ParamDescription("can be either 'color' or 'grayscale' for whether to load the image in color or grayscale mode. Defaults to 'color'.",true) },
	};

	return MetaData(
		"Load an image from a file.",
		"I/O",
		map<string, DataDescription>(), // no inputs
		output,
		params
	);
}

