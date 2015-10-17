#ifndef _CARTTOPOLARMODULE_
#define _CARTTOPOLARMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Applies Gaussian blurring to an image using openCV.   Then converts the image into grayscale
// See http://docs.opencv.org/modules/imgproc/doc/filtering.html#gaussianblur
class cartToPolarModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		cartToPolarModule(void): ModuleBase("cartToPolar"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~cartToPolarModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //CARTTOPOLARMODULE
