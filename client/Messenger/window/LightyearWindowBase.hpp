#ifndef LIGHTYEAR_WINDOWS_BASE_HPP
#define LIGHTYEAR_WINDOWS_BASE_HPP
#include <tuple>
#include "../DuiLib/UIlib.h"
using namespace DuiLib;

class LightyearWinodwBase : public DuiLib::WindowImplBase {
public:
    LightyearWinodwBase();
    virtual CDuiString GetSkinFolder() const;
    virtual UILIB_RESOURCETYPE GetResourceType() const;
    virtual CDuiString GetZIPFileName() const;
    virtual void InitTheme();
    virtual CDuiString GetWindowText() const = 0;
    virtual bool HasMaxButton() const { return true; }
    virtual bool HasSkinButton() const { return false; }
    virtual bool HasBackgroundImage() const{ return false; }
    virtual void UpdateTheme();

    static void ChangeTheme();

    std::tuple<std::wstring, DWORD> GetBackgroundImage();

private:
    void InitTitleBar();
    void InitBackground();
    static int bk_image_index_;
};

#endif