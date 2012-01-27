// std
#include <sstream>
#include <iostream>

// ops
#include "ops-log.h"

namespace ops
{

Log::Log()
{
}

TLogLevel& Log::level()
{
    static TLogLevel reportingLevel(logINFO);
    return reportingLevel;
}

void Log::setLevel(std::string newLevel){
    if (newLevel == "ERROR") {
        ops::Log::level() = ops::logERROR;
    } else if (newLevel == "WARNING") {
        ops::Log::level() = ops::logWARNING;
    } else if (newLevel == "INFO") {
        ops::Log::level() = ops::logINFO;
    } else if (newLevel == "DEBUG") {
        ops::Log::level() = ops::logDEBUG;
    } else {
        ops::Log::level() = ops::logDEBUG;
    }
}

std::string Log::toString(TLogLevel level)
{
    static const char* const buffer[] = {"ERROR  ", "WARNING", "INFO   ", "DEBUG  "};
    return buffer[level];
}

std::ostringstream& Log::get(TLogLevel level)
{
   //os << "- " << NowTime();
   os << toString(level) << " : ";
   return os;
}

Log::~Log()
{
    std::cerr << os.str() << std::endl;
}

} // namespace ops
