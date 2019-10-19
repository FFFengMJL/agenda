#include "AgendaService.hpp"

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
    auto index = [userName] (const User &user) {
        return userName == user.getName();
    };
    if (m_storage->queryUser(index).size() == 0) return false; // if not register
    Date start(startDate);
    Date end(endDate);
    // if (!Date::isValid(start) || !Date::isValid(end)) return false;
    if (start >= end || !Date::isValid(start) || !Date::isValid(end)) return false; // cannot be a time point
    auto filter = [userName, title, start, end](const Meeting &meeting) {
        return (meeting.getTitle() == title) || // title used
            (meeting.getStartDate() <= start && start < meeting.getEndDate()) || // cannot be create if have time joint
            (meeting.getStartDate() < end && end <= meeting.getEndDate());
    };
    if (m_storage->queryMeeting(filter).size() != 0) return false;
    m_storage->createMeeting(Meeting(userName, participator, start, end, title));
    return true;
    // return m_storage->sync();
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                            const std::string &title,
                            const std::string &participator) {
    auto filter = [userName, title, participator](const Meeting &meeting) {
        if (meeting.getSponsor() == userName && 
            meeting.getTitle() == title &&
            meeting.isParticipator(participator)) // if parts in meeting, don't add it
            return false;
        return true;
    };

    auto switcher = [participator](Meeting &meeting) {
        meeting.addParticipator(participator);
    };

    if (m_storage->updateMeeting(filter, switcher) == 0) return false;
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
        if (meeting.getTitle() == title && 
            meeting.isParticipator(userName) &&
            meeting.getSponsor() != userName) 
            return true;
        return false; // if parts in meeting, delete it
    };

    auto switcher = [userName](Meeting &meeting) {
        meeting.removeParticipator(userName);
    };

    if (m_storage->updateMeeting(filter, switcher) == 0) return false;
    // return m_storage->sync();
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const {
    auto filter = [userName, title](const Meeting &meeting) {
        if (meeting.getSponsor() == userName && 
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
    if (!Date::isValid(start) || !Date::isValid(end)) return k;
    auto filter = [userName, start, end] (const Meeting &meeting) {
        if (meeting.getSponsor() == userName && 
            meeting.getStartDate() <= start &&
            meeting.getEndDate() >= end)
            return true; // founded
        return false;
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