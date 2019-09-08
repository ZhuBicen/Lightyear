#include "RosterMgmt.hpp"
#include "log.hpp"
#include "Utility.hpp"
#include <boost/foreach.hpp> 
#include <algorithm> // for std::sort

static RosterMgmt* s_rosterMgmt = nullptr;

RosterMgmt::RosterMgmt(void)
{
}

RosterMgmt::~RosterMgmt(void)
{
}

void RosterMgmt::Reset() {
    if (s_rosterMgmt != nullptr) {
        delete s_rosterMgmt;
        s_rosterMgmt = new RosterMgmt;
    }
}
RosterItem RosterMgmt::Myself() {
    return m_myself;
}

bool RosterMgmt::AddRosterItem(const RosterItem& p_ri){
    // TODO: check the unique user id ?
    if (p_ri.id == m_myselfId) {
        m_myself = p_ri;
    }
    m_roster.push_back(p_ri);
    return true;
}
bool RosterMgmt::IsOnline(const std::wstring id) {
    auto ri = GetRosterItem(id);
    return ri.status == im::Presence_Show_AWAY || ri.status == im::Presence_Show_ONLINE;
}
void RosterMgmt::SetRosterItemStatus(const std::wstring id, im::Presence_Show p_show){
    GetRosterItem(id).status = p_show;
}

RosterMgmt* RosterMgmt::Instance() {
    if (s_rosterMgmt == nullptr) {
        s_rosterMgmt = new RosterMgmt();
        return s_rosterMgmt;
    }
    return s_rosterMgmt;
}

RosterItem& RosterMgmt::GetRosterItem(const std::wstring p_rosterNumber) {
    BOOST_FOREACH(RosterItem& l_rosterItem, m_roster) {
        if (l_rosterItem.id == p_rosterNumber) {
            return l_rosterItem;
        }
    }

    MLOG(logERROR) << "Invalid roster number found: " << ToString(p_rosterNumber);
    throw std::string("Invalid roster id found, fatal error.");
}


void RosterMgmt::Sort() {
    // TODO: remove this trick
    // Online users at the tail
    // UI add each Roster Item as the head
    // 
    std::sort(m_roster.begin(), m_roster.end(), 
        [](const RosterItem& lhs, const RosterItem& rhs) -> bool {

        if (lhs.status == im::Presence_Show_OFFLINE && rhs.status == im::Presence_Show_OFFLINE) {
            if (lhs.name && rhs.name) {
                return *lhs.name > *rhs.name;
            }
            return false;
        }
        if (lhs.status == im::Presence_Show_OFFLINE && rhs.status != im::Presence_Show_OFFLINE) {
            return true;
        }
        return false;

        });
}
