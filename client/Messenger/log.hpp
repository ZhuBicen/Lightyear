#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <stdio.h>

#include <windows.h>

inline std::string NowTime()
{
    const int MAX_LEN = 200;
	char buffer[MAX_LEN] = { 0 };
    if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, 
        "HH':'mm':'ss", buffer, MAX_LEN) == 0)
        return "Error in NowTime()";

	char result[MAX_LEN] = { 0 };
    static DWORD first = GetTickCount();
    sprintf_s(result, MAX_LEN - 1, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000); 
    return result;
}

enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

template <typename T>
class Log
{
public:
    Log();
    virtual ~Log();
    std::ostringstream& Get(const char*, int, TLogLevel level = logINFO);
public:
    static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);
protected:
    std::ostringstream os;
private:
    Log(const Log&);
    Log& operator =(const Log&);
};

template <typename T>
Log<T>::Log()
{
}

template <typename T>
std::ostringstream& Log<T>::Get(const char* file, int line, TLogLevel level )
{
	os << NowTime() << " - " ;
    os << ToString(level) << ": ";
    os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
    os << file << "#" << line << " ";
    return os;
}

template <typename T>
Log<T>::~Log()
{
    os << std::endl;
    T::Output(os.str());
}

template <typename T>
TLogLevel& Log<T>::ReportingLevel()
{
    static TLogLevel reportingLevel = logDEBUG4;
    return reportingLevel;
}

template <typename T>
std::string Log<T>::ToString(TLogLevel level)
{
	static const char* const buffer[] = {"ERR", "WAR", "INF", "DBG", "DG1", "DG2", "DG3", "DG4"};
    return buffer[level];
}

template <typename T>
TLogLevel Log<T>::FromString(const std::string& level)
{
    if (level == "DEBUG4")
        return logDEBUG4;
    if (level == "DEBUG3")
        return logDEBUG3;
    if (level == "DEBUG2")
        return logDEBUG2;
    if (level == "DEBUG1")
        return logDEBUG1;
    if (level == "DBG")
        return logDEBUG;
    if (level == "INF")
        return logINFO;
    if (level == "WAR")
        return logWARNING;
    if (level == "ERR")
        return logERROR;
    Log<T>().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}
class Output2Console
{
public:
    static void Output(const std::string& msg)
    {
        // fprintf(stderr, "%s", msg.c_str());
        // fflush(stderr);
        ::OutputDebugStringA(msg.c_str());
    }
};
//void Output2Console::Output(const std::string& msg)
//{
//    fprintf(stderr, "%s", msg.c_str());
//    fflush(stderr);
//}
//class Output2FILE
//{
//public:
//    static FILE*& Stream();
//    static void Output(const std::string& msg);
//};
//
//inline FILE*& Output2FILE::Stream()
//{
//    static FILE* pStream = stderr;
//    return pStream;
//}
//
//inline void Output2FILE::Output(const std::string& msg)
//{   
//    FILE* pStream = Stream();
//    if (!pStream)
//        return;
//    fprintf(pStream, "%s", msg.c_str());
//    fflush(pStream);
//}


class ConsoleLog : public Log<Output2Console> {};

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logDEBUG
#endif


#define MLOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > ConsoleLog::ReportingLevel()) ; \
    else ConsoleLog().Get(__FILE__, __LINE__, level)

#endif //__LOG_H__
