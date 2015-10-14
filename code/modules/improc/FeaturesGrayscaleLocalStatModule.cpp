#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FeaturesGrayscaleLocalStatModule.hpp"

using namespace std;
using namespace uipf;

/** @runs the module  **/
/**
data DataManager handles the input and output of this module
**/

void FeaturesGrayscaleLocalStatModule::run( DataManager& data) const
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

  int rows = img.rows;
  int cols = img.cols;
   
  Mat std = Mat(rows, cols, CV_32FC1);
  Mat pmr = Mat(rows, cols, CV_32FC1);
  Mat skw = Mat(rows, cols, CV_32FC1);
  Mat kur = Mat(rows, cols, CV_32FC1);
  Mat cnt = Mat(rows, cols, CV_32FC1);
  Mat hom = Mat(rows, cols, CV_32FC1);
  Mat tamcnt = Mat(rows, cols, CV_32FC1);
  Mat tamcoa = Mat(rows, cols, CV_32FC1);
  
  // pre-calc mean
  Mat mean;
  blur(img, mean, Size(boxSize, boxSize));

  int new_x, new_y;
  
  
  vector<Point2d> grid;
  double cur_mean, cur_var, cur_pmr, cur_skw, cur_kur, cur_cnt, cur_hom, cur_tamcnt;
  for(int y=0; y<img.rows; y++)
   {
	  float* img_row = img.ptr<float>(y);
	  float* mean_row = mean.ptr<float>(y);
      
	  float* std_row	 = std.ptr<float>(y);
	  float* pmr_row	 = pmr.ptr<float>(y);
	  float* skw_row	 = skw.ptr<float>(y);
	  float* kur_row	 = kur.ptr<float>(y);
	  float* cnt_row	 = cnt.ptr<float>(y);
	  float* hom_row	 = hom.ptr<float>(y);
	  float* tamcnt_row	 = tamcnt.ptr<float>(y);
      
	  for(int x=0; x<img.cols; x++)
	  {
	      cur_mean = mean_row[x];
	      cur_var = cur_skw = cur_kur = cur_cnt = cur_hom = 0;
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
		  
				  float value = img.at<float>(new_y,new_x);
		  
				  cur_var += pow((value - cur_mean),2);
				  cur_skw += pow((value - cur_mean),3);
				  cur_kur += pow((value - cur_mean),4);
				  if (cur_mean > eps) cur_cnt += pow((value - img_row[x])/cur_mean,2);
				  if (cur_mean > eps) cur_hom += 1.0/(1.0+pow((value - img_row[x])/cur_mean,2));
			  }
	      }
	      cur_var /= boxSize*boxSize-1;
	      if (cur_mean > eps)
			cur_pmr = sqrt(cur_var) / cur_mean;
	      else
			cur_pmr = 0;
	      if (cur_var > eps)
	      {
			  cur_skw /= boxSize*boxSize*pow(cur_var, 1.5);
			  cur_kur /= boxSize*boxSize*pow(cur_var, 2);
	      }
	      else
	      {
			cur_skw = 0;
			cur_kur = 0;
	      }
	      cur_cnt /= boxSize*boxSize-1;
	      cur_hom /= boxSize*boxSize-1;
	      if (cur_kur > eps)
	      {
			cur_tamcnt = sqrt(cur_var)/pow(cur_kur, 0.25);
	      }
	      else
	      {
			cur_tamcnt = 0;
	  	  }
	      std_row[x] = sqrt(cur_var);
	      pmr_row[x] = cur_pmr;
	      skw_row[x] = cur_skw;
	      kur_row[x] = cur_kur;
	      cnt_row[x] = cur_cnt;
	      hom_row[x] = cur_hom;
	      tamcnt_row[x] = cur_tamcnt;
	  }
     
  }
  
  int maxK = 5;
  int new_x_0, new_x_1, new_y_0, new_y_1;
  double e_kv, e_kh;
  Mat e_max = Mat::zeros(rows, cols, CV_32FC1);
  for(int k=1; k<maxK; k++)
  {
      blur(img, mean, Size(pow(2,2*(k-1)), pow(2,2*(k-1))));
        for(int y=0; y<img.rows; y++)
        {
	      float* e_max_row   = e_max.ptr<float>(y);
	      float* tamcoa_row  = tamcoa.ptr<float>(y);
	      for(int x=0; x<img.cols; x++)
	      {
		  // get new coordinates
		  new_y_0 = y + pow(2,2*(k-1));
		  new_y_1 = y - pow(2,2*(k-1));
		  new_x_0 = x + pow(2,2*(k-1));
		  new_x_1 = x - pow(2,2*(k-1));
		  // check boundary
		  if (new_y_0 < 0) new_y_0 = 0;
		  if (new_y_0 >= img.rows) new_y_0 = img.rows-1;
		  if (new_y_1 < 0) new_y_1 = 0;
		  if (new_y_1 >= img.rows) new_y_1 = img.rows-1;
		  if (new_x_0 < 0) new_x_0 = 0;
		  if (new_x_0 >= img.cols) new_x_0 = img.cols-1;
		  if (new_x_1 < 0) new_x_1 = 0;
		  if (new_x_1 >= img.cols) new_x_1 = img.cols-1;
		
		  e_kh = abs(mean.at<float>(y,new_x_0) - mean.at<float>(y,new_x_1));
		  e_kv = abs(mean.at<float>(new_y_0,x) - mean.at<float>(new_y_1,x));
	      
		  if (e_kv >= e_max.at<float>(y, x))
		  {
		      e_max_row[x] = e_kv;
		      tamcoa_row[x] = k;
		  }
		  if (e_kh >= e_max.at<float>(y, x))
		  {
		      e_max_row[x] = e_kh;
		      tamcoa_row[x] = k;
		  }
	     }
	  }
	}

	// set the result (output) on the datamanager
  data.setOutputData("std", new Matrix(std));
  data.setOutputData("pmr", new Matrix(pmr));
  data.setOutputData("skw", new Matrix(skw));
  data.setOutputData("kur", new Matrix(kur));
  data.setOutputData("cnt", new Matrix(cnt));
  data.setOutputData("hom", new Matrix(hom));
  data.setOutputData("tamcoa", new Matrix(tamcoa));
  data.setOutputData("tamcnt", new Matrix(tamcnt));
 
}

// returns the meta data of this module
MetaData FeaturesGrayscaleLocalStatModule::getMetaData() const
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
		{"tamcoa", DataDescription(MATRIX, "the result image.") },
		{"tamcnt", DataDescription(MATRIX, "the result image.") }
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
