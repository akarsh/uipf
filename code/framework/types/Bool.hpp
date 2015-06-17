#ifndef BOOL_H
#define BOOL_H


#include "Data.hpp"

namespace uipf{


// Float which is a specification of Elem
class Bool : Data {
	public:
			typedef SMARTPOINTER<Bool> ptr;
			typedef const SMARTPOINTER<Bool> c_ptr;

	public:
		// constructor (can't be virtual!)
		Bool(void){};
		// destructor
		~Bool(void){};
		
		// get content
		bool getContent();
		
		// set content
		void setContent(bool);
		
	private:
		// content
		bool b_;
};

}

#endif
