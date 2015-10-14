#ifndef _LOADIMAGETWOMODULE_
#define _LOADIMAGETWOMODULE_

#include "../../framework/ModuleInterface.hpp"
#include "../../framework/ModuleBase.hpp"

namespace uipf{


class LoadImageTwoModule :  public QObject, ModuleBase
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor tells ModuleBase our name so we don't need to implement name()
		LoadImageTwoModule(void): ModuleBase("loadImageTwo"){};

		// destructor needs to be virtual otherwise it not called due polymorphism
		virtual ~LoadImageTwoModule(void){};

		// runs the module chain with the params
		void run( DataManager& data ) const Q_DECL_OVERRIDE;

		uipf::MetaData getMetaData() const Q_DECL_OVERRIDE;

};

}

#endif //LOADIMAGETWOMODULE
