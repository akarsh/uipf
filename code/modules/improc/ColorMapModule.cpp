#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ColorMapModule.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void ColorMapModule::run( DataManager& data) const
{
	using namespace cv;

	// print params for debugging
	data.listParams();

	// Variables
	Mat m_0;
	Mat m_1;
	Mat m_2,m_3,m_4,m_5,m_6,m_7,m_8,m_9,m_10,m_11;

	// get a pointer to the "image" input data
	Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
	// get the actual opencv matrix of the input data
	Mat m = oMatrix->getContent();

	applyColorMap(m, m_0, COLORMAP_AUTUMN);
	// applyColorMap(m, m_1, COLORMAP_BONE);
	// applyColorMap(m, m_2, COLORMAP_JET);
	// applyColorMap(m, m_3, COLORMAP_WINTER);
	// applyColorMap(m, m_4, COLORMAP_RAINBOW);
	// applyColorMap(m, m_5, COLORMAP_OCEAN);
	// applyColorMap(m, m_6, COLORMAP_SUMMER);
	// applyColorMap(m, m_7, COLORMAP_SPRING);
	// applyColorMap(m, m_8, COLORMAP_COOL);
	// applyColorMap(m, m_9, COLORMAP_HSV);
	// applyColorMap(m, m_10, COLORMAP_PINK);
	// applyColorMap(m, m_11, COLORMAP_HOT);

	// set the result (output) on the datamanager
	data.setOutputData("COLORMAP_AUTUMN",new Matrix(m_0));
	// data.setOutputData("COLORMAP_BONE",new Matrix(m_1));
	// data.setOutputData("COLORMAP_JET",new Matrix(m_2));
	// data.setOutputData("COLORMAP_WINTER",new Matrix(m_3));
	// data.setOutputData("COLORMAP_RAINBOW",new Matrix(m_4));
	// data.setOutputData("COLORMAP_OCEAN",new Matrix(m_5));
	// data.setOutputData("COLORMAP_SUMMER",new Matrix(m_6));
	// data.setOutputData("COLORMAP_SPRING",new Matrix(m_7));
	// data.setOutputData("COLORMAP_COOL",new Matrix(m_8));
	// data.setOutputData("COLORMAP_HSV",new Matrix(m_9));
	// data.setOutputData("COLORMAP_PINK",new Matrix(m_10));
	// data.setOutputData("COLORMAP_HOT",new Matrix(m_11));

}

// returns the meta data of this module
MetaData ColorMapModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"COLORMAP_AUTUMN", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_BONE", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_JET", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_WINTER", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_RAINBOW", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_OCEAN", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_SUMMER", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_SPRING", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_COOL", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_HSV", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_PINK ", DataDescription(MATRIX, "the result image.") },
		// {"COLORMAP_HOT", DataDescription(MATRIX, "the result image.") }

	};
	map<string, ParamDescription> params = {};

	return MetaData(
		"Applies color map to an image using openCV.",
		"Image Processing",
		input,
		output,
		params
	);
}
