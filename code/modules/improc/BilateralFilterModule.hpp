#ifndef _BILATERALFILTERMODULE_
#define _BILATERALFILTERMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Applies Bilateral Filter to an image using openCV.
// See http://docs.opencv.org/doc/tutorials/imgproc/gausian_median_blur_bilateral_filter/gausian_median_blur_bilateral_filter.html#code
class BilateralFilterModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		BilateralFilterModule(void): ModuleBase("bilateral filter"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~BilateralFilterModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //BILATERALFILTERMODULE
