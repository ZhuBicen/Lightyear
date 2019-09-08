#include "control/RecentRosterList.hpp"
#include "control/RosterLogoButton.hpp"
#include "RosterMgmt.hpp"
#include "RosterItemUI.hpp"
#include "Log.hpp"
#include "proto/src/im.pb.h"

namespace DuiLib
{

const int kFriendListItemNormalHeight = 32;
const int kFriendListItemSelectedHeight = 50;
CRecentRosterList::CRecentRosterList(CPaintManagerUI& paint_manager)
	: delay_deltaY_(0)
	, delay_number_(0)
	, delay_left_(0)
	, level_expand_image_(_T("<i list_icon_b.png>"))
	, level_collapse_image_(_T("<i list_icon_a.png>"))
	, level_text_start_pos_(10)
	, text_padding_(10, 0, 0, 0)
	, paint_manager_(paint_manager)
{
	SetItemShowHtml(true);
}

CRecentRosterList::~CRecentRosterList()
{
}

bool CRecentRosterList::Add(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;

	return CListUI::Add(pControl);
}

bool CRecentRosterList::AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;

	return CListUI::AddAt(pControl, iIndex);
}

bool CRecentRosterList::Remove(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;

	return CListUI::Remove(pControl);
}

bool CRecentRosterList::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;
	return CListUI::RemoveAt(iIndex);
}

void CRecentRosterList::RemoveAll()
{
	CListUI::RemoveAll();
}

void CRecentRosterList::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
			m_pParent->DoEvent(event);
		else
			CVerticalLayoutUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID)
	{
		if (delay_left_ > 0)
		{
			--delay_left_;
			SIZE sz = GetScrollPos();
			LONG lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
			if ((lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ))
			{
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		delay_deltaY_ = 0;
		delay_number_ = 0;
		delay_left_ = 0;
		m_pManager->KillTimer(this, SCROLL_TIMERID);
		return;
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		LONG lDeltaY = 0;
		if (delay_number_ > 0)
			lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
		switch (LOWORD(event.wParam))
		{
		case SB_LINEUP:
			if (delay_deltaY_ >= 0)
				delay_deltaY_ = lDeltaY + 8;
			else
				delay_deltaY_ = lDeltaY + 12;
			break;
		case SB_LINEDOWN:
			if (delay_deltaY_ <= 0)
				delay_deltaY_ = lDeltaY - 8;
			else
				delay_deltaY_ = lDeltaY - 12;
			break;
		}
		if
			(delay_deltaY_ > 100) delay_deltaY_ = 100;
		else if
			(delay_deltaY_ < -100) delay_deltaY_ = -100;

		delay_number_ = (DWORD)sqrt((double)abs(delay_deltaY_)) * 5;
		delay_left_ = delay_number_;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 50U);
		return;
	}

	CListUI::DoEvent(event);
}


CControlUI* CRecentRosterList::CreateControl(LPCTSTR pstrClass) {
    if (_tcsicmp(pstrClass, _T("RosterLogoButton")) == 0)
    {
        return new CRosterLogoButton(m_pManager);
    }
    return NULL;
}
CListContainerElementUI* CRecentRosterList::AddRosterItem(const RosterItem& item)
{
    // create the list item
    auto l_rosterElement = CreateListContainerElementUI(item);
	//l_rosterElement->SetFixedHeight(kFriendListItemSelectedHeight);
    return AddListContainerElementUI(item, l_rosterElement);
}

void CRecentRosterList::UpdateRosterItem(const RosterItem& p_ri){
    // remove the old one
    RemoveAt(roster_[p_ri.id]->GetIndex());
    // create the new one, to show the online users firstly
    AddListContainerElementUI(p_ri, CreateListContainerElementUI(p_ri));
    SelectItem(0, false);
}
void CRecentRosterList::AddRecentRosterItem(const RosterItem& ri) {
    // Update existing one
    if (roster_.find(ri.id) != roster_.end()) {
        UpdateRosterItem(ri);
        return;
    }
    // Add new one
    // auto l_rosterElement = CreateListContainerElementUI(ri);
    AddRosterItem(ri);
}

void CRecentRosterList::ChangeRecenRosterItemOnlineStauts(const RosterItem& ri){
    if (roster_.find(ri.id) != roster_.end()) {
        // to change the online status.
        static_cast<CRosterItemUI*>(roster_[ri.id])->UpdateOnlineStatus(ri.status);
    }
}
CListContainerElementUI* CRecentRosterList::CreateListContainerElementUI(const RosterItem &item)
{
    return new CRosterItemUI(*m_pManager, item);
}


// append as the first item
CListContainerElementUI* CRecentRosterList::AddListContainerElementUI(const RosterItem &item, CListContainerElementUI * l_rosterElement)
{
   if (!CListUI::AddAt(l_rosterElement, 0)) {
        delete l_rosterElement;
        return nullptr;
   }
    // each roster item UI element has embedded ID
    roster_[item.id] = l_rosterElement;
    l_rosterElement->SetTag(reinterpret_cast<UINT_PTR>(new CDuiString(item.id.c_str())));
    return l_rosterElement;
}

} // namespace DuiLib
