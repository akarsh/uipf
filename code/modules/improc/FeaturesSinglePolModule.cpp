#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FeaturesSinglePolModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void FeaturesSinglePolModule::run( DataManager& data) const
{
  using namespace cv;

  // print params for debugging
  data.listParams();

  // get a pointer to the "image" input data
  Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
  // get the actual opencv matrix of the input data
  Mat img = oMatrix->getContent();

  // read the params (window size and sigma) given for this step
  int winSize = data.getParam<int>("winSize", 11);

  int looks = 1;
  double sig2  = 1.0/looks;
  
  Mat ampl;
  if (img.channels() == 2){
      vector<Mat> channels;
      split(img, channels);
      Mat phase;
      cartToPolar(channels[0], channels[1], ampl, phase);
  }else
      ampl = img.clone();
      
  // variation coefficient
  Mat mean, mean_squared, mean_of_squares, var, std, tmp1, vc_s;
  Mat ampl2,var2,mean2, tmp1_1, tmp2_1,std_1;
  blur(ampl, mean, Size(winSize, winSize));
  
  ampl.convertTo(ampl2, CV_32F);
  pow(ampl2, 2, mean_of_squares);
  blur(mean_of_squares, mean_of_squares, Size(winSize, winSize));
 
  var = mean_of_squares - mean_squared;
  var.convertTo(var2, CV_32F);
  pow(var2, 0.5, std);

  mean.convertTo(mean2, CV_32F);
  tmp1.convertTo(tmp1_1, CV_32F);
  std.convertTo(std_1, CV_32F);
  divide(std, mean2, tmp1_1);
 
  vc_s = tmp1_1.clone();
    
  tmp1_1 = mean_squared*sig2;

  tmp1_1 = var2 - tmp1_1;
 
  Mat tmp2 = mean_squared * pow(sig2,2);
  tmp2.convertTo(tmp2_1, CV_32F);

  tmp2_1 = var2 + tmp2_1;

  divide(tmp1_1, tmp2_1, tmp2_1);
  Mat k_s = tmp2_1.clone();
  
  // variation coefficient
  winSize = winSize*2;

  blur(ampl, mean, Size(winSize, winSize));
  mean.convertTo(mean2,CV_32F);
  pow(mean2, 2, mean_squared);

  pow(ampl2, 2, mean_of_squares);
  blur(mean_of_squares, mean_of_squares, Size(winSize, winSize));

  var = mean_of_squares - mean_squared;
  pow(var2, 0.5, std_1);

  divide(std_1, mean2, tmp1_1);
  Mat vc_b = tmp1_1.clone();

  tmp1_1 = mean_squared*sig2;
  tmp1_1 = var2 - tmp1_1;
  tmp2_1 = mean_squared*pow(sig2,2);
  tmp2_1 = var2 + tmp2_1;

  divide(tmp1_1, tmp2_1, tmp2_1);
  Mat k_b = tmp2_1.clone();
  
  Mat k_d = k_b - k_s;;
  Mat vc_d = vc_b - vc_s;
  
	// set the result (output) on the datamanager
	data.setOutputData("vcSmall",new Matrix(vc_s));
	data.setOutputData("kSmall",new Matrix(k_s));
	data.setOutputData("vcBig",new Matrix(vc_b));
	data.setOutputData("kBig",new Matrix(k_b));
	data.setOutputData("vcDiff",new Matrix(vc_d));
	data.setOutputData("kDiff",new Matrix(k_d));

}

// returns the meta data of this module
MetaData FeaturesSinglePolModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"vcSmall", DataDescription(MATRIX, "the result image.") },
		{"kSmall", DataDescription(MATRIX, "the result image.") },
		{"vcBig", DataDescription(MATRIX, "the result image.") },
		{"kBig", DataDescription(MATRIX, "the result image.") },
		{"vcDiff", DataDescription(MATRIX, "the result image.") },
		{"kDiff", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"winSize", ParamDescription("window size of the kernel.", true) }
	};

	return MetaData(
		"Show features of the image",
		"Image Processing",
		input,
		output,
		params
	);
}
