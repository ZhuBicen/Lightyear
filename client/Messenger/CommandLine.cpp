#include "CommandLine.hpp"
#include "Log.hpp"
#include <boost/program_options.hpp>

CommandLine* CommandLine::s_instance = nullptr;
CommandLine::CommandLine(void)
{
    m_autoSignin = std::make_tuple(false, L"", L"");
}


CommandLine::~CommandLine(void)
{
}

CommandLine* CommandLine::Instance(){
    if (!s_instance)
    {
        s_instance = new CommandLine();
        s_instance->parse();
    }
    return s_instance;
}
CommandLine::AutoSigninInfo
CommandLine::GetAutoSignin(){
        return m_autoSignin;
}

bool CommandLine::parse(void)
{
    namespace po = boost::program_options;

    po::options_description l_desc("Options"); 
    l_desc.add_options() 
        ("help", "Print help messages") 
        ("id", po::wvalue<std::wstring>(), "ID to signin") 
        ("pw", po::wvalue<std::wstring>(), "Password corresponding the ID"); 


    auto l_cmdString = GetCommandLine();
    std::vector<std::wstring> l_args = po::split_winmain(l_cmdString);
    po::variables_map l_vm; 

    try {
        po::store(po::basic_command_line_parser<wchar_t> (l_args).options(l_desc).run(), l_vm);
        if (l_vm.count("id") && l_vm.count("pw")){

            MLOG(logDEBUG) << __FUNCTION__ << " auto signin enabled";
            m_autoSignin = std::make_tuple(true,
                l_vm["id"].as<std::wstring>(),
                l_vm["pw"].as<std::wstring>()
                );
        }
    }
    catch(po::error& e) 
    { 
        MLOG(logERROR) << "Exception caught:" << e.what();
        MLOG(logERROR) << "Please specify the command line argument as:";
        MLOG(logERROR) << l_desc;
    } 
    catch(...) {

        MLOG(logERROR) << __FUNCTION__ << " Unknow exception.";
    }
    return false;
}
