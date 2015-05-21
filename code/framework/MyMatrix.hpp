#ifndef MYMATRIX_H
#define MYMATRIX_H


#include "MyMatrix.hpp"

// Matrix which is a specification of Elem
class MyMatrix : Elem {
	public:
		// constructor (can't be virtual!)
		MyMatrix(void){};
		// destructor
		~MyMatrix(void){};
		
		// get content
		Mat getContent();
		
		// set content
		void setContent(Mat&);
		
	private:
		// content
		Mat matrix;
};


#endif
