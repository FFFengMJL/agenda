#include "AgendaService.hpp"
#include <iostream>

AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

void AgendaService::startAgenda() {
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    m_storage->sync();
}

bool AgendaService::userLogIn(const std::string &userName,
                              const std::string &password) {
    auto filter = [userName, password](const User &user) {
        return user.getName() == userName && user.getPassword() == password;
    };
    if (m_storage->queryUser(filter).size() == 0) return false;
    return true;
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                  const std::string &email, const std::string &phone) {
    auto filter = [userName] (const User &user) {
        return user.getName() == userName;
    };
    if (m_storage->queryUser(filter).size() != 0) return false;
    m_storage->createUser(User(userName, password, email, phone));
    return true;
    // return m_storage->sync();
}


// something wrong
bool AgendaService::deleteUser(const std::string &userName, 
                               const std::string &password) {
    auto filter = [userName, password] (const User &user) {
        return user.getName() == userName && user.getPassword() == password;
    };
    // user unexisted
    if (m_storage->deleteUser(filter) == 0) return false;
    
    // user existed
    auto filter1 = [userName] (const Meeting &meeting) {
        return meeting.getSponsor() == userName ||
               meeting.getParticipator().size() == 0;
    };

    auto filter2 = [userName] (const Meeting &meeting) {
        return meeting.isParticipator(userName);
    };
    auto switch2 = [userName] (Meeting &meeting) {
        meeting.removeParticipator(userName);
    };

    m_storage->updateMeeting(filter2, switch2);
    m_storage->deleteMeeting(filter1);
    return true;
    // return m_storage->sync();
}

std::list<User> AgendaService::listAllUsers(void) const {
    auto filter = [](const User &user) {return true;};
    return m_storage->queryUser(filter);
}

// something wrong but i don't know how to solve it
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                   const std::string &startDate, const std::string &endDate,
                   const std::vector<std::string> &participator) {
    if (userName == "" || participator.size() == 0) return false; // no sponsor
    // else {
    //     for (auto i = participator.begin(); i != participator.end(); i++) {
    //         auto t = i;
    //         t++;
    //         // participator repeat
    //         // sponsor == participator
    //         for (; t != participator.end(); t++) {
    //             if (*i == userName || *i == *t) return false;
    //         }
    //     }
    // }

    // participator register
    auto filter = [participator, userName] (const User &user) {
        for (auto i = participator.begin(); i != participator.end(); i++) {
            if (*i == user.getName() || user.getName() == userName) return true;
        }
        return false;
    };
    if (m_storage->queryUser(filter).size() != participator.size() + 1) return false;

    Date start(startDate);
    Date end(endDate);
    // time cannot be a time point
    // time must be valid
    if (start >= end || !Date::isValid(start) || !Date::isValid(end)) return false;
    auto filter1 = [userName, title, start, end](const Meeting &meeting) {
        // std::cout << "filter1" << std::endl;
        return (meeting.getTitle() == title) || // title used
            // sponsor time joint
            ((meeting.isParticipator(userName) || meeting.getSponsor() == userName) && 
            !(meeting.getStartDate() >= end || start >= meeting.getEndDate()));
    };

    auto filter2 = [participator, start, end] (const Meeting &meeting) {
        // std::cout << "filter2" << std::endl;
        for (auto i = participator.begin(); i != participator.end(); i++) {
            // participator have time
            if ((meeting.isParticipator(*i) || meeting.getSponsor() == *i)&& 
                !(meeting.getStartDate() >= end || start >= meeting.getEndDate()))
                return true;
        }
        return false;
    };

    if (m_storage->queryMeeting(filter1).size() != 0 || m_storage->queryMeeting(filter2).size() != 0) 
        return false;
    m_storage->createMeeting(Meeting(userName, participator, start, end, title));
    return true;
    // return m_storage->sync();
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                            const std::string &title,
                            const std::string &participator) {
    // participator must be register
    auto index = [participator] (const User &user) {
        return participator == user.getName();
    };
    if (m_storage->queryUser(index).size() == 0) return false;

    // 找到目标会议，并且该会议不能包含即将添加的参与者
    auto filter2 = [userName, title, participator] (const Meeting meeting) {
        return meeting.getSponsor() == userName && 
               meeting.getTitle() == title && 
              !meeting.isParticipator(participator);
    };
    std::list<Meeting> tar = m_storage->queryMeeting(filter2);
    if (tar.size() == 0)return false;
    Date start = tar.begin()->getStartDate();
    Date end = tar.begin()->getEndDate();

    // 寻找即将参与者 已经参与的 会议中是否和目标会议有时间冲突
    // auto filter = [start, end, participator, title] (const Meeting meeting) {
    //     if (title != meeting.getTitle() && // 非目标会议
    //         (meeting.getSponsor() == participator || meeting.isParticipator(participator)) && 
    //         // part在此会议中
    //         !(meeting.getStartDate() >= end || meeting.getEndDate() <= start)) // 时间冲突
    //         return true;
    //     return false;
    // };
    // if (m_storage->queryMeeting(filter).size() != 0) return false;
    tar = listAllMeetings(participator);
    for (auto i = tar.begin(); i != tar.end(); i++) {
        if (!(i->getEndDate() <= start || i->getStartDate() >= end)) return false;
    }

    auto switcher = [participator](Meeting &meeting) {
        meeting.addParticipator(participator);
    };
    if (m_storage->updateMeeting(filter2, switcher) == 0) return false;
    return true;
    // return m_storage->sync();
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
    auto filter = [userName, title, participator](const Meeting &meeting) {
        if (meeting.getSponsor() == userName && 
            meeting.getTitle() == title &&
            meeting.isParticipator(participator)) // if parts in meeting, delete it
            return true;
        return false;
    };

    auto switcher = [participator](Meeting &meeting) {
        meeting.removeParticipator(participator);
    };

    if (m_storage->updateMeeting(filter, switcher) == 0) return false;
    return true;
    // return m_storage->sync();
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    auto filter = [title, userName](const Meeting &meeting) {
        return meeting.getTitle() == title && 
            meeting.isParticipator(userName) &&
            meeting.getSponsor() != userName;
            // if parts in meeting, delete it
    };

    auto switcher = [userName](Meeting &meeting) {
        meeting.removeParticipator(userName);
    };

    if (m_storage->updateMeeting(filter, switcher) == 0) return false;

    //删除没有参与者的会议
    auto filter1 = [userName] (const Meeting &meeting) {
        return meeting.getParticipator().size() == 0;
    };
    m_storage->deleteMeeting(filter1);
    // return m_storage->sync();
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const {
    auto filter = [userName, title](const Meeting &meeting) {
        if ((meeting.getSponsor() == userName || 
            meeting.isParticipator(userName)) && 
            meeting.getTitle() == title) 
            return true; 
        return false;
    };

    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                              const std::string &startDate,
                                              const std::string &endDate) const {
    Date start = Date(startDate);
    Date end = Date(endDate);
    std::list<Meeting> k;
    // startDate and endDate are not valid
    if (!Date::isValid(start) || !Date::isValid(end)) return k;
    auto filter = [userName, start, end] (const Meeting &meeting) {
        return ((meeting.getSponsor() == userName || 
                meeting.isParticipator(userName)) && 
                !(meeting.getStartDate() > end || meeting.getEndDate() < start));
    };

    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    auto filter = [userName] (const Meeting &meeting) {
        return meeting.getSponsor() == userName || meeting.isParticipator(userName);
    };

    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    auto filter = [userName] (const Meeting &meeting) {
        return meeting.getSponsor() == userName;
    };

    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
    
    auto filter = [userName] (const Meeting &meeting) {
        return meeting.getSponsor() != userName && meeting.isParticipator(userName);
    };

    return m_storage->queryMeeting(filter);
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    auto filter = [userName, title] (const Meeting &meeting) {
        return meeting.getTitle() == title && meeting.getSponsor() == userName;
    };

    if (m_storage->deleteMeeting(filter) == 0) return false;
    return true;
    // return m_storage->sync();
}

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string &userName) {
    auto filter = [userName] (const Meeting &meeting) {
        return meeting.getSponsor() == userName;
    };

    if (m_storage->deleteMeeting(filter) == 0) return false;
    return true;
    // return m_storage->sync();
}