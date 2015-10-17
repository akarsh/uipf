#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FeaturesGrayscaleGradHistModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void FeaturesGrayscaleGradHistModule::run( DataManager& data) const
{
  using namespace cv;

  // print params for debugging
  data.listParams();

  // get a pointer to the "image" input data
  Matrix::c_ptr oMatrix = data.getInputData<Matrix>("image");
  // get the actual opencv matrix of the input data
  Mat img = oMatrix->getContent();

  float eps = 0.001;

  int boxSize = 11;

  int scale = 1;
  int delta = 0;
  int ksize=3;
  int dtype=-1;
  int threshold_value = 0;
  int max_BINARY_value=0;

  int rows = img.rows;
  int cols = img.cols;
  
  Mat mean(rows, cols, CV_32FC1);
  Mat std(rows, cols, CV_32FC1);
  Mat pmr(rows, cols, CV_32FC1);
  Mat skw(rows, cols, CV_32FC1);
  Mat kur(rows, cols, CV_32FC1);
  Mat cnt(rows, cols, CV_32FC1);
  Mat hom(rows, cols, CV_32FC1);
    
  // pre-calc grad
  Mat grad_x, grad_y, grad_amp, grad_orient;
  //Sobel(InputArray src, OutputArray dst, int ddepth, int dx, int dy, int ksize=3, double scale=1,  double delta=0, int borderType=BORDER_DEFAULT );
  Sobel(img, grad_x, CV_32FC1, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
  Sobel(img, grad_y, CV_32FC1, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
  
  // cartToPolar(InputArray x, InputArray y, OutputArray magnitude, OutputArray angle, bool angleInDegrees=false)
  cartToPolar(grad_x, grad_y, grad_amp, grad_orient, false);
 
  Mat orient_mean, orient_norm;
 
  // multiply(InputArray src1, InputArray src2, OutputArray dst, double scale=1, int dtype=-1 )
  multiply(grad_amp, grad_orient, orient_mean, scale, dtype);
  
  //blur(InputArray src, OutputArray dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT )
  blur(orient_mean, orient_mean, Size(boxSize, boxSize), Point(-1,-1),BORDER_DEFAULT);

//threshold( src_gray, dst, threshold_value, max_BINARY_value,threshold_type );
  threshold(orient_mean, orient_mean, threshold_value, max_BINARY_value, CV_THRESH_TOZERO);

  blur(grad_amp, orient_norm, Size(boxSize, boxSize), Point(-1,-1),BORDER_DEFAULT);
  threshold(orient_norm, orient_norm, threshold_value, max_BINARY_value, CV_THRESH_TOZERO);
  

  for(int r=0; r<orient_mean.rows; r++)
  {
	for(int c=0; c<orient_mean.cols; c++)
	{
		float wSum = orient_mean.at<float>(r, c);
		float norm = orient_norm.at<float>(r, c);
		float val;
		if (norm > eps)
			val = wSum / norm;
		else
			val = 0;
		orient_mean.at<float>(r, c) = val;
	}
  }  
		
  double cur_norm, cur_mean, cur_var, cur_pmr, cur_skw, cur_kur, cur_cnt, cur_hom;
  int new_x, new_y;

  	for(int y=0; y<img.rows; y++)
   {
   		  float* mean_row	= mean.ptr<float>(y);
	      float* std_row	= std.ptr<float>(y);
	      float* pmr_row	= pmr.ptr<float>(y);
	      float* skw_row	= skw.ptr<float>(y);
	      float* kur_row	= kur.ptr<float>(y);
	      float* cnt_row    = cnt.ptr<float>(y);
	      float* hom_row	= hom.ptr<float>(y);
  		for(int x=0; x<img.cols; x++)
	  {
	      cur_norm = cur_mean = cur_var = cur_skw = cur_kur = cur_cnt = cur_hom = 0;
	  	  float cur_mean = orient_mean.at<float>(y,x);
	      float cur_norm = orient_norm.at<float>(y,x);
	      for(int i = -floor(boxSize/2.); i < ceil(boxSize/2.); i++)
	      {
			  for(int j = -floor(boxSize/2.); j < ceil(boxSize/2.); j++)
			  {
				  // get new coordinates
				  new_y = y+i;
				  new_x = x+j;
				  // check boundary
				  if (new_y < 0) new_y = 0;
				  if (new_y >= img.rows) new_y = img.rows-1;
				  if (new_x < 0) new_x = 0;
				  if (new_x >= img.cols) new_x = img.cols-1;
				  
				  float cur_amp = grad_amp.at<float>(new_y,new_x);
				  float cur_ori = grad_orient.at<float>(new_y,new_x);
		  
				  cur_var += cur_amp*pow((cur_ori - cur_mean),2);
				  cur_skw += cur_amp*pow((cur_ori - cur_mean),3);
				  cur_kur += cur_amp*pow((cur_ori - cur_mean),4);
				  if (cur_mean > eps) cur_cnt += cur_amp*pow((cur_ori - grad_orient.at<float>(y,x))/cur_mean,2);
				  if (cur_mean > eps) cur_hom += cur_amp/(1.0+pow((cur_ori - grad_orient.at<float>(y,x))/cur_mean,2));
			  }
	      }

	      if (cur_mean > eps)
	      {
			  cur_pmr = sqrt(cur_var) / cur_mean;
	      }
	      else
	      {
			cur_pmr = 0;
		  }

	      if (cur_var > eps)
	      {
			  cur_skw /= cur_norm*pow(cur_var, 1.5);
			  cur_kur /= cur_norm*pow(cur_var, 2);
	      }
	      else
	      {
			  cur_skw = 0;
			  cur_kur = 0;
	      }

	      if (cur_norm > eps){
			  cur_cnt /= cur_norm;
			  cur_hom /= cur_norm;
		  }
		  else
		  {
			  cur_cnt = 0;
			  cur_hom = 0;
		  }
		   
	      mean_row[x] = cur_mean;
	      std_row[x] = sqrt(cur_var);
	      pmr_row[x] = cur_pmr;
	      skw_row[x] = cur_skw;
	      kur_row[x] = cur_kur;
	      cnt_row[x] = cur_cnt;
	      hom_row[x] = cur_hom;
	    }
	 }
    
  	// set the result (output) on the datamanager
	data.setOutputData("mean",new Matrix(mean));
	data.setOutputData("std",new Matrix(std));
	data.setOutputData("pmr",new Matrix(pmr));
	data.setOutputData("skw",new Matrix(skw));
	data.setOutputData("kur",new Matrix(kur));
	data.setOutputData("cnt",new Matrix(cnt));
	data.setOutputData("hom",new Matrix(hom));
}

// returns the meta data of this module
MetaData FeaturesGrayscaleGradHistModule::getMetaData() const
{
	map<string, DataDescription> input = {
		{"image", DataDescription(MATRIX, "the image to apply the filter on.") }
	};
	map<string, DataDescription> output = {
		{"std", DataDescription(MATRIX, "the result image.") },
		{"pmr", DataDescription(MATRIX, "the result image.") },
		{"skw", DataDescription(MATRIX, "the result image.") },
		{"kur", DataDescription(MATRIX, "the result image.") },
		{"cnt", DataDescription(MATRIX, "the result image.") },
		{"hom", DataDescription(MATRIX, "the result image.") },
		{"mean", DataDescription(MATRIX, "the result image.") }
	};
	map<string, ParamDescription> params = {
		{"boxSize", ParamDescription("boxSize of the image") }	
	};

	return MetaData(
		"Show features of the image",
		"Image Processing",
		input,
		output,
		params
	);
}
