#ifndef LIGHTYEAR_MESSENGER_ROSTER_MGMT_HPP
#define LIGHTYEAR_MESSENGER_ROSTER_MGMT_HPP
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "proto/src/im.pb.h"
// #include "MessageHistory.hpp"

struct RosterItem
{
    std::wstring  id;  // bare JID,
    im::Presence_Show status;
    boost::optional<std::wstring>  logo;
    boost::optional<std::wstring>  name;
    boost::optional<std::wstring>  position;
    boost::optional<std::wstring>  department;  // 所属部门
};

class RosterMgmt
{
private:
	RosterMgmt(void);
public:
	~RosterMgmt(void);
    RosterItem Myself();
    bool AddRosterItem(const RosterItem&);
    RosterItem& GetRosterItem(const std::wstring id);
    void SetRosterItemStatus(const std::wstring id, im::Presence_Show);
    bool IsOnline(const std::wstring id);

    // sort roster items by sso_name
    void Sort();

    static RosterMgmt* Instance();
    static void Reset();

    std::vector<RosterItem>& GetRoster() {return m_roster;}
    void SetMyselfId(const std::wstring& id) {
        m_myselfId = id;
    }

    void SetMyselfResource(const std::wstring& resource) {
        m_myselfResource = resource;
    }
    const std::wstring GetMyselfResource() {
        return m_myselfResource;
    }

private:
	RosterItem m_myself;
    std::wstring m_myselfId;
	std::wstring m_myselfResource;

    std::vector<RosterItem> m_roster;

};

#endif