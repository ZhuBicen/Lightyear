#include "LightyearWindowBase.hpp"
#include "Log.hpp"
#include <sstream>

int LightyearWinodwBase::bk_image_index_ = 0;
LightyearWinodwBase::LightyearWinodwBase() {

}
CDuiString LightyearWinodwBase::GetSkinFolder() const {
#ifdef _DEBUG
    // Debug mode depends on the skin folder in Messenger dir
    // TODO: check skin dir exist under current working directory
    //       to make the debug mode binary workable in-depently.
    return _T("../../LightyearClient/Messenger/skin");
#else 
    return _T("");
#endif
}

UILIB_RESOURCETYPE LightyearWinodwBase::GetResourceType() const
{
#ifdef _DEBUG
    return UILIB_FILE;
#else
    return UILIB_ZIPRESOURCE;
#endif 
}

CDuiString LightyearWinodwBase::GetZIPFileName() const
{
#ifdef _DEBUG
    return _T("skin.zip");
#else
    return _T("");
#endif
}
void LightyearWinodwBase::UpdateTheme() {
    InitBackground();
}
std::tuple<std::wstring, DWORD> LightyearWinodwBase::GetBackgroundImage()
{
    static std::vector<DWORD> colors{ 0xFF1D1A3A, 0xFFFBCD3B, 0xFF969C3C, 0xFF8FACA9, 0xFFFCFC9C, 0xFFE6E8E3 };
    std::wostringstream wos;
    if (HasBackgroundImage()) {
        wos << _T("file = 'background/bg") << this->bk_image_index_ << _T(".png' dest='0,0,300,150'");
    }
    else {
        wos << _T("file = 'background/bg") << this->bk_image_index_ << _T(".png' corner='684,346,1,1'");
    }
    MLOG(logINFO) << "Skin changed to:" << this->bk_image_index_;
    return std::make_tuple(wos.str(), colors[this->bk_image_index_]);
}
void LightyearWinodwBase::InitTheme() {
    InitTitleBar();
    InitBackground();
}
void LightyearWinodwBase::InitBackground() {
    CHorizontalLayoutUI* bg = (CHorizontalLayoutUI*)m_PaintManager.FindControl(_T("LightyearBackground"));
    if (bg != NULL) {
        bg->SetBkImage(std::get<0>(GetBackgroundImage()).c_str());
        bg->SetBkColor(std::get<1>(GetBackgroundImage()));
    }
    CVerticalLayoutUI* bd = (CVerticalLayoutUI*)m_PaintManager.FindControl(_T("LightyearBorder"));
    if (bd != NULL) {
        bd->SetBkImage(_T("file = 'background/border.png' corner = '5,5,5,5'"));
        bd->SetInset(RECT{ 1, 1, 1, 1 });
    }
}

void LightyearWinodwBase::InitTitleBar()
{
    CHorizontalLayoutUI* l_titleBar = (CHorizontalLayoutUI*)m_PaintManager.FindControl(_T("LightyearTitleBar"));
    if (l_titleBar == NULL) {
        return;
    }
    l_titleBar->SetFixedHeight(30);
    // l_titleBar->SetBorderColor(0xFFFFFFFF);
    // l_titleBar->SetBorderSize(3);

    CVerticalLayoutUI* l_titleTextLayout = new CVerticalLayoutUI;
    //l_titleTextLayout->SetFixedWidth(60);
    CLabelUI* l_titleText = new CLabelUI();
    l_titleText->SetText(GetWindowText());
    l_titleText->SetTextColor(0xFFFFFFFF);
    l_titleTextLayout->Add(l_titleText);

    l_titleBar->Add(l_titleTextLayout);

    //CVerticalLayoutUI* l_dummy = new CVerticalLayoutUI;
    //l_titleBar->Add(l_dummy);

    CHorizontalLayoutUI* l_titleButtons = new CHorizontalLayoutUI;
    l_titleButtons->SetFixedWidth(94);
    l_titleButtons->SetBorderColor(0xFFFFFFFF);

    std::vector<CButtonUI*> l_titleVec;

    if (HasSkinButton()) {
        CButtonUI* l_skinButton = new CButtonUI;
        l_skinButton->SetFloat(true);
        l_skinButton->SetName(_T("bkskinbtn"));
        l_skinButton->SetFixedWidth(28);
        l_skinButton->SetFixedHeight(19);
        l_skinButton->SetMaxWidth(28);
        l_skinButton->SetMaxHeight(19);
        l_skinButton->SetNormalImage(_T("title\\skin.png"));
        l_skinButton->SetHotImage(_T("title\\skin.png"));
        l_skinButton->SetPushedImage(_T("title\\skin.png"));
        l_skinButton->SetVisible(true);
        l_titleVec.push_back(l_skinButton);
    }

    // min button
    CButtonUI* l_minButton = new CButtonUI;
    l_minButton->SetFloat(true);
    l_minButton->SetName(_T("minbtn"));
    l_minButton->SetFixedWidth(28);
    l_minButton->SetFixedHeight(19);
    l_minButton->SetMaxWidth(28);
    l_minButton->SetMaxHeight(19);
    l_minButton->SetNormalImage(_T("title\\btn_mini_normal.png"));
    l_minButton->SetHotImage(_T("title\\btn_mini_highlight.png"));
    l_minButton->SetPushedImage(_T("title\\btn_mini_down.png"));
    l_minButton->SetVisible(true);
    l_titleVec.push_back(l_minButton);

    if (HasMaxButton()) {
        // max button
        CButtonUI* l_maxButton = new CButtonUI;
        l_maxButton->SetFloat(true);
        l_maxButton->SetName(_T("maxbtn"));
        l_maxButton->SetFixedWidth(28);
        l_maxButton->SetFixedHeight(19);
        l_maxButton->SetMaxWidth(28);
        l_maxButton->SetMaxHeight(19);
        l_maxButton->SetNormalImage(_T("title\\btn_max_normal.png"));
        l_maxButton->SetHotImage(_T("title\\btn_max_highlight.png"));
        l_maxButton->SetPushedImage(_T("title\\btn_max_down.png"));
        l_maxButton->SetVisible(true);
        l_titleVec.push_back(l_maxButton);
    }
    // close button
    CButtonUI* l_closeButton = new CButtonUI;
    l_closeButton->SetFloat(true);
    l_closeButton->SetName(_T("closebtn"));
    l_closeButton->SetFixedWidth(39);
    l_closeButton->SetFixedHeight(19);
    l_closeButton->SetMaxWidth(39);
    l_closeButton->SetMaxHeight(19);
    l_closeButton->SetNormalImage(_T("title\\btn_close_normal.png"));
    l_closeButton->SetHotImage(_T("title\\btn_close_highlight.png"));
    l_closeButton->SetPushedImage(_T("title\\btn_close_down.png"));
    l_closeButton->SetVisible(true);
    l_titleVec.push_back(l_closeButton);

    l_titleButtons->SetFixedWidth(l_titleVec.size() * 28 + 11);
    std::vector<SIZE> l_sizes = { SIZE{ 0, 0 }, SIZE{ 28, 0 }, SIZE{ 56, 0 }, SIZE{ 84, 0 } };
    for (std::vector<SIZE>::size_type i = 0; i < l_titleVec.size(); i++) {
        l_titleVec[i]->SetFixedXY(l_sizes[i]);
        l_titleButtons->Add(l_titleVec[i]);
        if (l_titleVec[i]->GetName() == L"maxbtn") {
            // restore button
            CButtonUI* l_restoreButton = new CButtonUI;
            l_restoreButton->SetFloat(true);
            l_restoreButton->SetName(_T("restorebtn"));
            l_restoreButton->SetFixedWidth(28);
            l_restoreButton->SetFixedHeight(19);
            l_restoreButton->SetMaxWidth(28);
            l_restoreButton->SetMaxHeight(19);
            l_restoreButton->SetNormalImage(_T("title\\btn_restore_normal.png"));
            l_restoreButton->SetHotImage(_T("title\\btn_restore_highlight.png"));
            l_restoreButton->SetPushedImage(_T("title\\btn_restore_down.png"));
            l_restoreButton->SetVisible(false);
            l_restoreButton->SetFixedXY(l_sizes[i]);
            l_titleButtons->Add(l_restoreButton);
        }
    }
    l_titleBar->Add(l_titleButtons);

}

void LightyearWinodwBase::ChangeTheme()
{
    bk_image_index_ = (bk_image_index_ + 1) % 6;
}
