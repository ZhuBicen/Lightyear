#include "Utility.hpp"
#undef min
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <Shlwapi.h>

std::string Base64Enocde(const BYTE* buffer, long size) {
    using namespace boost::archive::iterators;
    // Crazy typedef black magic
    typedef
        base64_from_binary<    // convert binary values to base64 characters
        transform_width< const unsigned char *, 6, 8  >
        >
        base64Iterator; // compose all the above operations in to a new iterator

    // Encode the buffer and create a string
    return std::string(
        base64Iterator(buffer),
        base64Iterator(buffer + size));

    return "";
}

std::wstring GenerateScreenshotFileName() {
    SYSTEMTIME time = { 0 };
    wchar_t fileName[MAX_PATH] = { 0 };
    ::GetLocalTime(&time);
    swprintf_s(fileName, MAX_PATH, L"%04d%02d%02d_%02d%02d%02d.bmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
    return fileName;
}

std::wstring GetBinaryPathW() {
    static std::wstring binaryPath;
    if (binaryPath.empty() ) {
        wchar_t dest[MAX_PATH] = { 0 };
        DWORD length = GetModuleFileNameW(NULL, dest, MAX_PATH);
        PathRemoveFileSpecW(dest);
        binaryPath = dest;
    }
    return binaryPath;
}

std::string GetBinaryPathA() {
    static std::string binaryPath;
    if (binaryPath == "") {
        char dest[MAX_PATH];
        DWORD length = GetModuleFileNameA(NULL, dest, MAX_PATH);
        PathRemoveFileSpecA(dest);
        binaryPath = dest;
    }
    return binaryPath;
}