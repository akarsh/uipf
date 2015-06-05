#ifndef LOGGER_H
#define LOGGER_H

//shortcuts to log
//can be used like LOG_E("something bad happened!")
#define LOG_E(message) Logger::instance()->Error(message)
#define LOG_I(message) Logger::instance()->Info(message)
#define LOG_W(message) Logger::instance()->Warn(message)

#include <sstream>

namespace uipf
{


//This is a very simple Logger implemented as singleton, that writes to console
//2DO: use a loggingframework e.g. Log4QT
class Logger 
{
	public:
     		static Logger* instance();

	   private:
	     static Logger *instance_;
	     Logger() {}
	     Logger( const Logger& );

	     ~Logger() {}

	 	class Guard {
			 public: ~Guard() {
			   if( Logger::instance_ != 0 )
			     delete Logger::instance_;
			 }
		     };
     		friend class Guard;

	public:
			
		void Warn(const std::string& strMessage);
		void Error(const std::string& strMessage);
		void Info(const std::string& strMessage);
	private:
		
		void print(const std::stringstream& what);
		
};

} //namespace

#endif // LOGGER_H
