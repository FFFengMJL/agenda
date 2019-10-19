#include "Meeting.hpp"

Meeting::Meeting(const std::string &t_sponsor,
          const std::vector<std::string> &t_participator,
          const Date &t_startTime, const Date &t_endTime,
          const std::string &t_title) {
              m_sponsor = t_sponsor;
              this->setParticipator(t_participator);
              m_startDate = t_startTime;
              m_endDate = t_endTime;
              m_title = t_title;
}

Meeting::Meeting(const Meeting &t_meeting) {
    m_sponsor = t_meeting.getSponsor();
    this->setParticipator(t_meeting.getParticipator());
    m_startDate = t_meeting.getStartDate();
    m_endDate = t_meeting.getEndDate();
    m_title = t_meeting.getTitle();
}

// 5 getter
std::string Meeting::getSponsor() const {
    return m_sponsor;
}

std::vector<std::string> Meeting::getParticipator() const {
    return m_participators;
}

Date Meeting::getStartDate() const {
    return m_startDate;
}

Date Meeting::getEndDate() const {
    return m_endDate;
}

std::string Meeting::getTitle() const {
    return m_title;
}

// 5 setting
void Meeting::setSponsor(const std::string &t_sponsor) {
    m_sponsor = t_sponsor;
}

void Meeting::setParticipator(const std::vector<std::string> &t_participator) {
    m_participators.clear();
    for (int i = 0; i < t_participator.size(); i++) {
        m_participators.push_back(t_participator[i]);
    }
}

void Meeting::setStartDate(const Date &t_startTime) {
    m_startDate = t_startTime;
}

void Meeting::setEndDate(const Date &t_endTime) {
    m_endDate = t_endTime;
}

void Meeting::setTitle(const std::string &t_title) {
    m_title = t_title;
}

bool Meeting::isParticipator(const std::string &t_username) const {
    for (int i = 0; i < m_participators.size(); i++) {
        if(m_participators[i] == t_username) return true;
    }
    return false;
}

void Meeting::addParticipator(const std::string &t_participator) {
    m_participators.push_back(t_participator);
}

void Meeting::removeParticipator(const std::string &t_participator) {
    for (auto i = m_participators.begin(); i != m_participators.end(); ) {
        if(*i == t_participator) {
            auto tar = i;
            i = m_participators.erase(i);
            continue;
        }
        i ++;
    }
}