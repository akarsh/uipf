#ifndef _HISTOGRAMEQUALIZATIONMODULE_
#define _HISTOGRAMEQUALIZATIONMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{

// Gives Histogram Equalization to an image using openCV.
// http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_equalization/histogram_equalization.html
class HistogramEqualizationModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		HistogramEqualizationModule(void): ModuleBase("histogramequalization"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~HistogramEqualizationModule(void){};

		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //HISTOGRAMEQUALIZATIONMODULE
