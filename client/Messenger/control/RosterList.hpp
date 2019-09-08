#ifndef UIFRIENDS_HPP
#define UIFRIENDS_HPP
#include <map>
#include "Utility.hpp"

namespace im{
    enum Presence_Show;
}
struct RosterItem;

namespace DuiLib
{

class CRosterList : public CListUI
{
public:

	enum {SCROLL_TIMERID = 10};

	CRosterList(CPaintManagerUI& paint_manager);

	~CRosterList();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();

	void DoEvent(TEventUI& event);

	// 
	CListContainerElementUI* AddRosterItem(const RosterItem&);

    CListContainerElementUI* AddListContainerElementUI(const RosterItem &item, CListContainerElementUI * l_rosterElement);

    void AddDepartmentIfNeeded(const RosterItem &item);

    CListContainerElementUI* CreateListContainerElementUI(const RosterItem &item);

    void UpdateRosterItem(const RosterItem& ri);
    bool RotateDepartmentAndEmployee(std::wstring department);
	

	bool IsDepartment(std::wstring); 

private:
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
	std::map<std::wstring, CListContainerElementUI*> departments_; 
	std::map<std::wstring, CListContainerElementUI*> roster_; /// number -> roterItem

	CListContainerElementUI* AddDepartment(const std::wstring&, int index);
	void RotateDepartmentIcon(std::wstring p_deparmentName, bool expanded) ;
};

} // DuiLib

#endif // UIFRIENDS_HPP
