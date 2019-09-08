#include "control/RosterList.hpp"
#include "control/RosterLogoButton.hpp"
#include "RosterMgmt.hpp"
#include "RosterItemUI.hpp"
#include "Log.hpp"
#include "proto/src/im.pb.h"

namespace DuiLib
{

const int kFriendListItemNormalHeight = 32;
const int kFriendListItemSelectedHeight = 50;
CRosterList::CRosterList(CPaintManagerUI& paint_manager)
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

CRosterList::~CRosterList()
{
}

bool CRosterList::Add(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;

	return CListUI::Add(pControl);
}

bool CRosterList::AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;

	return CListUI::AddAt(pControl, iIndex);
}

bool CRosterList::Remove(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;

	return CListUI::Remove(pControl);
}

bool CRosterList::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("RosterListItemUI")) != 0)
		return false;
	return CListUI::RemoveAt(iIndex);
}

void CRosterList::RemoveAll()
{
	CListUI::RemoveAll();
}

void CRosterList::DoEvent(TEventUI& event) 
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

CListContainerElementUI* CRosterList::AddRosterItem(const RosterItem& item)
{
    // create the list item
    auto l_rosterElement = CreateListContainerElementUI(item);
	//l_rosterElement->SetFixedHeight(kFriendListItemSelectedHeight);
    AddDepartmentIfNeeded(item);
    
    return AddListContainerElementUI(item, l_rosterElement);
}
const TCHAR* const kNameControlName = _T("name");
CListContainerElementUI* CRosterList::AddDepartment(const std::wstring& p_department, int index){

	TCHAR szBuf[MAX_PATH] = {0};

	CListContainerElementUI* pListElement = NULL;
	if( !m_dlgBuilderDepartment.GetMarkup()->IsValid() ) {
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilderDepartment.Create(_T("xml\\friend_list_item_department.xml"), (UINT)0, NULL, &paint_manager_));
	}
	else {
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilderDepartment.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return nullptr;

	pListElement->SetVisible(true);

	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNameControlName));
	if (nick_name != NULL) {	
        CDuiString html_text;
        html_text += level_expand_image_;
        _stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
        html_text += szBuf;
        html_text += p_department.c_str();
		nick_name->SetFixedWidth(0);
		nick_name->SetShowHtml(true);
		nick_name->SetText(html_text);
    }

	pListElement->SetFixedHeight(kFriendListItemNormalHeight);
	if (!CListUI::Add(pListElement))
	{
		delete pListElement;
	}
	//
	departments_[p_department] = pListElement;
	pListElement->SetTag(reinterpret_cast<UINT_PTR>(new CDuiString(p_department.c_str())));
	return pListElement;
}
bool CRosterList::IsDepartment(std::wstring p_departmentName){
	return departments_.find(p_departmentName) != departments_.end();
}

bool CRosterList::RotateDepartmentAndEmployee(std::wstring department){
	if (departments_.find(department) == departments_.end()) {
		return false;
	}
	auto l_childVisible = false;
    // iterator all the employees under the specified department.
	for(RosterItem& l_listItem: RosterMgmt::Instance()->GetRoster()){
		if (l_listItem.department == department) {
            if (roster_.find(l_listItem.id) == roster_.end()) {
                // in case that: myself is not showed in the roster gui list.
                continue;
            }
			CListContainerElementUI* l_selectedItem = roster_[l_listItem.id];
			CControlUI* control = GetItemAt(l_selectedItem->GetIndex());
			control->SetVisible(!control->IsVisible());
			l_childVisible = control->IsVisible();
			// l_selectedItem->SetVisible(!l_selectedItem->IsVisible());
		}
	}
	RotateDepartmentIcon(department, l_childVisible);
    return true;
}

void CRosterList::RotateDepartmentIcon(std::wstring p_deparmentName, bool expanded) {
	auto l_departmentListItem = departments_[p_deparmentName];

	CDuiString html_text;
	if (expanded) {
		html_text += level_expand_image_;
	}else {
		html_text += level_collapse_image_;
	}
	TCHAR szBuf[MAX_PATH] = {0};
	// <x i>  表示从此处向后面移动 x 个像素值
	_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);

	html_text += szBuf;

	html_text += p_deparmentName.c_str();


	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(l_departmentListItem, kNameControlName));
	if (nick_name != NULL)
	{
		nick_name->SetFixedWidth(0);
		nick_name->SetShowHtml(true);
		nick_name->SetText(html_text);
	}
}

void CRosterList::UpdateRosterItem(const RosterItem& p_ri){
    // remove the old one
    RemoveAt(roster_[p_ri.id]->GetIndex());
    // create the new one, to show the online users firstly
    AddListContainerElementUI(p_ri, CreateListContainerElementUI(p_ri));
}

CListContainerElementUI* CRosterList::CreateListContainerElementUI(const RosterItem &item) {
    return new CRosterItemUI(*m_pManager, item);
}

void CRosterList::AddDepartmentIfNeeded(const RosterItem &item)
{

    if (item.department) {
        auto l_departmentListItem = departments_.find(*item.department);
        if (l_departmentListItem == departments_.end()) { // this is the first roster under this department
            AddDepartment(*item.department, 0);
        }
    }
}

// append as the first item
CListContainerElementUI* CRosterList::AddListContainerElementUI(const RosterItem &item, CListContainerElementUI * l_rosterElement)
{
   auto l_index = 0; // if the item has no department, append it to the head
   if (item.department) {
       l_index = departments_[*item.department]->GetIndex() + 1; // new added roster item is the first child of the department
   } 
   if (!CListUI::AddAt(l_rosterElement, l_index)) {
        delete l_rosterElement;
        return nullptr;
   }
    // each roster item UI element has embedded ID
    roster_[item.id] = l_rosterElement;
    l_rosterElement->SetTag(reinterpret_cast<UINT_PTR>(new CDuiString(item.id.c_str())));
    return l_rosterElement;
}

} // namespace DuiLib
