#ifndef _CALCULATEHISTMODULE_
#define _CALCULATEHISTMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Calculates Histogram fo an image using openCV.
// http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
class CalculateHistModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		CalculateHistModule(void): ModuleBase("calculate histogram"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~CalculateHistModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //CALCULATEHISTMODULE
