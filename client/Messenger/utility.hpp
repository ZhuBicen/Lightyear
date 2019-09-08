#ifndef LIGHTYEAR_UTILITY_HPP
#define LIGHTYEAR_UTILITY_HPP
#include <boost/locale.hpp>
#include "../DuiLib/UiLib.h"
using namespace DuiLib;

inline double CalculateDelay(double state) {
    return pow(state, 2);
}

inline std::wstring ToWstring(const std::string& p_utf8string) {
	return boost::locale::conv::to_utf<wchar_t>(p_utf8string,"utf-8");
}

inline std::string ToString(const std::wstring& p_utf16string) {
    return boost::locale::conv::utf_to_utf<char>(p_utf16string);
}

inline std::string ConvertDuiToUtf8(const CDuiString& p_duiString) {
    return ToString(p_duiString.GetData());
}
std::string Base64Enocde(const BYTE* buffer, long size);

std::wstring GenerateScreenshotFileName();

std::string GetBinaryPathA();
std::wstring GetBinaryPathW();
#endif