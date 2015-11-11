#include "ShowRatModule.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../framework/types/Matrix.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/random.hpp>
#include <boost/random/gamma_distribution.hpp>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace uipf;

/*

*/
void ShowRatModule::run( DataManager& data) const
{
	using namespace cv;

    List<Matrix>::c_ptr pList = data.getInputData< List<Matrix> >("channels");
    list<Matrix::ptr> list = pList->getContent();
    
    bool dual = false;
    // to single-pol: use individual channels
    double minVal, maxVal;
    Mat tmp;
    // to color: use channels from speckle reduced image
    // load speckle-reduced image; produced with RAT until speckle-reduction C/C++-Code is available
    // check if dual-pol or full-pol
    vector<Mat> cov;
    for(auto it = list.begin(); it != list.end(); ++it)
    {
     cov.push_back((*it)->getContent());
    }

    Mat blue, blue1, blue2, red, green, span, colImg_small, colImg;
    vector<Mat> channels;
    // get channel info
    if(cov.size()==2)
    {
     split(cov.at(0), channels);
     red = channels.at(0).clone();
     split(cov.at(1), channels);
     green = channels.at(0).clone();
     blue = (red+green)/2;
    }
    else if(cov.size()==3)
    {
     split(cov.at(0), channels);
     red = channels.at(0).clone();
     split(cov.at(1), channels);
     green = channels.at(0).clone();
     split(cov.at(2), channels);
     blue = channels.at(0).clone();
    }
    else if(cov.size()==4)
    {
     split(cov.at(0), channels);
     red = channels.at(0).clone();
     split(cov.at(1), channels);
     green = channels.at(0).clone();
     split(cov.at(2), channels);
     blue1 = channels.at(0).clone();
     split(cov.at(3), channels);
     blue2 = channels.at(0).clone();
     blue = (blue1+blue2)/2;
    }
    else if(cov.size()==9)
    {
     split(cov.at(0), channels);
     red = channels.at(0).clone();
     split(cov.at(4), channels);
     green = channels.at(0).clone();
     split(cov.at(8), channels);
     blue = channels.at(0).clone();
    }

//    if (this->path.find("oph") != string::npos){
//    LOG_E("use log");
   red = red + 1;
   green = green + 1;
   blue = blue + 1;
//    log(red, red);
//    log(green, green);
//    log(blue, blue);

//    LOG_E("use pow");
//    Mat blue2, green2, red2;
//    Mat blue3,green3,red3;
    
//    blue.convertTo(blue3, CV_32F);
//    green.convertTo(green3,CV_32F);
//    red.convertTo(red3, CV_32F);
//
//  pow(blue3, 0.5, blue2);
//  pow(green3, 0.5, green2);
//  pow(red3, 0.5, red2);

    threshold(red, red, 2.5*mean(red).val[0], 0, THRESH_TRUNC);
    threshold(green, green, 2.5*mean(green).val[0], 0, THRESH_TRUNC);
    threshold(blue, blue, 2.5*mean(blue).val[0], 0, THRESH_TRUNC);

//     threshold(red2, red2, 2.5*mean(red2).val[0], 0, THRESH_TRUNC);
//     threshold(green2, green2, 2.5*mean(green2).val[0], 0, THRESH_TRUNC);
//     threshold(blue2, blue2, 2.5*mean(blue2).val[0], 0, THRESH_TRUNC);


    max(red, green, tmp);
    max(blue, tmp, tmp);

//   max(red2, green2, tmp);
//   max(blue2, tmp, tmp);

    minMaxLoc(red, &minVal, &maxVal);
    red = red * 255./maxVal;
    minMaxLoc(green, &minVal, &maxVal);
    green = green * 255./maxVal;
    minMaxLoc(blue, &minVal, &maxVal);
    blue = blue * 255./maxVal;
    channels.clear();
    channels.push_back(blue.clone());
    channels.push_back(green.clone());
    channels.push_back(red.clone());

//    minMaxLoc(red2, &minVal, &maxVal);
//    red2 = red2 * 255./maxVal;
//    minMaxLoc(green2, &minVal, &maxVal);
//    green2 = green2 * 255./maxVal;
//    minMaxLoc(blue2, &minVal, &maxVal);
//    blue2 = blue2 * 255./maxVal;
//    channels.clear();
//    channels.push_back(blue2.clone());
//    channels.push_back(green2.clone());
//    channels.push_back(red2.clone());

    // merge color planes to image

    merge(channels, colImg_small);
    // NOTE: SPECKLE-REDUCED IMAGE MIGHT BE SMALLER THAN ORIGINAL IMAGE ==> re-size!!

    resize(colImg_small, colImg, Size(cov.at(0).cols, cov.at(0).rows));

    data.setOutputData("image", new Matrix(colImg));

    Mat grad_x, grad_y, amp, angle, binary, binary1;
    int scale = 1;
    int delta = 0;
    
    Sobel(colImg, grad_x, -1, 1, 0, CV_SCHARR);
    Sobel(colImg, grad_y, -1, 0, 1, CV_SCHARR);
    cartToPolar(grad_x, grad_y, amp, angle);
    binary1 = amp > 0.5*mean(amp).val[0];

    binary.push_back(binary1.clone());

    data.setOutputData("image2", new Matrix(binary));
      
}

MetaData ShowRatModule::getMetaData() const
{
    map<string, DataDescription> input = {
        {"channels", DataDescription(MATRIX_LIST, "the image channel list.") }
    };
    map<string, DataDescription> output = {
        {"image", DataDescription(MATRIX, "the image as a result from the merge.") },
        {"image2", DataDescription(MATRIX, "the image as a result from the merge.") }
    };
	map<string, ParamDescription> params = {};

	return MetaData(
		"Show an image by opening a window.",
		"I/O",
		input,
        output,
		params
	);
}

