#ifndef __OPS_LOG_H_INCLUDED
#define __OPS_LOG_H_INCLUDED

// std
#include <sstream>


namespace ops
{

	enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG};

	class Log
	{
	public:
	   Log();
	   virtual ~Log();
	   std::ostringstream& get(TLogLevel level = logINFO);
	public:
	   static TLogLevel& level();
	   static void setLevel(std::string newLevel);
	protected:
	   std::string toString(TLogLevel level);
	   std::ostringstream os;
	private:
	   Log(const Log&);
	   Log& operator =(const Log&);
	};




} // namespace ops



#define ERROR \
if (ops::logERROR > ops::Log::level()) ; \
else ops::Log().get(ops::logERROR) << __PRETTY_FUNCTION__ << " : "

#define WARNING \
if (ops::logWARNING > ops::Log::level()) ; \
else ops::Log().get(ops::logWARNING) << __PRETTY_FUNCTION__ << " : "

#define INFO \
if (ops::logINFO > ops::Log::level()) ; \
else ops::Log().get(ops::logINFO) << __PRETTY_FUNCTION__ << " : "

#define DEBUG \
if (ops::logDEBUG > ops::Log::level()) ; \
else ops::Log().get(ops::logDEBUG) << __PRETTY_FUNCTION__ << " : "

#endif // __OPS_LOG_H_INCLUDED

