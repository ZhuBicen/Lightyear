#pragma once
#include <string>
#include <tuple>
class CommandLine
{
public:
    typedef std::tuple<bool, std::wstring, std::wstring> AutoSigninInfo;

    CommandLine();
    ~CommandLine();
    static CommandLine* Instance();
    AutoSigninInfo GetAutoSignin();

private:
    static CommandLine* s_instance;
    AutoSigninInfo m_autoSignin;
public:
    bool parse(void);
};

