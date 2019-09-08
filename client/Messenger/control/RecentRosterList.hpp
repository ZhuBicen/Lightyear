#ifndef RECENT_ROSTER_LIST_HPP
#define RECENT_ROSTER_LIST_HPP
#include <map>
#include "Utility.hpp"

namespace im{
    enum Presence_Show;
}
struct RosterItem;

namespace DuiLib
{

class CRecentRosterList : public CListUI, IDialogBuilderCallback
{
public:
    virtual CControlUI* CreateControl(LPCTSTR pstrClass);
public:

	enum {SCROLL_TIMERID = 10};

    CRecentRosterList(CPaintManagerUI& paint_manager);

    ~CRecentRosterList();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();

	void DoEvent(TEventUI& event);



    // 
    void AddRecentRosterItem(const RosterItem& ri);
    void ChangeRecenRosterItemOnlineStauts(const RosterItem& ri);

private:
	// 
	CListContainerElementUI* AddRosterItem(const RosterItem&);
    void UpdateRosterItem(const RosterItem& ri);
    CListContainerElementUI* AddListContainerElementUI(const RosterItem &item, CListContainerElementUI * l_rosterElement);
    CListContainerElementUI* CreateListContainerElementUI(const RosterItem &item);
    
    LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;
	CDuiRect	text_padding_;
	int level_text_start_pos_;
	CDuiString level_expand_image_;
	CDuiString level_collapse_image_;
	CPaintManagerUI& paint_manager_;

    CDialogBuilder m_dlgBuilder;
    CDialogBuilder m_dlgBuilderDepartment;

private:
	std::map<std::wstring, CListContainerElementUI*> roster_; /// number -> roterItem
};

} // DuiLib

#endif // UIFRIENDS_HPP
