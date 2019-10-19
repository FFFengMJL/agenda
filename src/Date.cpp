#include "Date.hpp"

Date::Date() {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
    // if (isValid(*this)) m_year = m_month = m_day = m_hour = m_minute = 0;
}

Date::Date(const std::string &dateString) {
    m_year = m_month = m_day = m_hour = m_minute = 0;
    bool flag = false;
    if (dateString.length() == 16 
     && dateString[4] == '-' 
     && dateString[7] == '-'
     && dateString[10] == '/'
     && dateString[13] == ':') {
        for (int i = 0; i < 4; i ++) {
            int temp = dateString[i] - '0';
            if (temp < 10 && temp >= 0) 
            m_year = m_year * 10 + temp;
            else {
                flag = true;
                break;
            }
        }
        int a = 0, b = 0;

        a = dateString[5] - '0';
        b = dateString[6] - '0';
        if (a < 10 && a >=0 && b >=0 && b < 10)
            m_month = (dateString[5] - '0') * 10 + dateString[6] - '0';
        else flag = true;

        a = dateString[8] - '0';
        b = dateString[9] - '0';
        if (a < 10 && a >=0 && b >=0 && b < 10)
            m_day = (dateString[8] - '0') * 10 + dateString[9] - '0';
        else flag = true;

        a = dateString[11] - '0';
        b = dateString[12] - '0';
        if (a < 10 && a >=0 && b >=0 && b < 10)
            m_hour = (dateString[11] - '0') * 10 + dateString[12] - '0';
        else flag = true;

        a = dateString[14] - '0';
        b = dateString[15] - '0';
        if (a < 10 && a >=0 && b >=0 && b < 10)
            m_minute = (dateString[14] - '0') * 10 + dateString[15] - '0';
        else flag = true;
    }
    if (flag) m_year = m_month = m_day = m_hour = m_minute = 0;
}

// year
int Date::getYear() const {
    return m_year;
}

void Date::setYear(const int t_year) {
    m_year = t_year;
}

// month
int Date::getMonth() const {
    return m_month;
}

void Date::setMonth(const int t_month) {
    m_month = t_month;
}

// day
int Date::getDay() const {
    return m_day;
}

void Date::setDay(const int t_day) {
    m_day = t_day;
}

// hour
int Date::getHour() const {
    return m_hour;
}

void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}

// minute
int Date::getMinute() const {
    return m_minute;
}

void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}

bool Date::isValid(const Date &t_date) {
    if (t_date.getYear() < 1000 || t_date.getYear() > 9999) return false;
    if (t_date.getHour() < 0 || t_date.getHour() > 23) return false;
    if (t_date .getMinute() < 0 || t_date.getMinute() > 59) return false;
    switch(t_date.getMonth()) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            if (t_date.getDay() < 1 || t_date.getDay() > 31) return false;break;
        case 2:
            if ((t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0) ||
                t_date.getYear() % 400 == 0) {
                    if (t_date.getDay() < 1 || t_date.getDay() > 29) return false;break;
                }
            else if (t_date.getDay() < 1 || t_date.getDay() > 28) return false;break;
        case 4: case 6: case 9: case 11:
            if (t_date.getDay() < 1 || t_date.getDay() > 30) return false;break;
        default: return false;
    }
    return true;
}

Date Date::stringToDate(const std::string &t_dateString) {
    Date temp(t_dateString);
    return temp;
}

std::string Date::dateToString(const Date &t_date) {
    if (!Date::isValid(t_date)) return "0000-00-00/00:00";
    std::string s_year = std::to_string(t_date.getYear());
    while (s_year.length() != 4) {
        s_year = "0" + s_year;
    }
    std::string s_mon = std::to_string(t_date.getMonth());
    if (t_date.getMonth() < 10) s_mon = "0" + s_mon;
    std::string s_day = std::to_string(t_date.getDay());
    if (t_date.getDay() < 10) s_day = "0" + s_day;
    std::string s_hour = std::to_string(t_date.getHour());
    if (t_date.getHour() < 10) s_hour = "0" + s_hour;
    std::string s_min = std::to_string(t_date.getMinute());
    if (t_date.getMinute() < 10) s_min = "0" + s_min;
    return s_year + "-" + s_mon + "-" + s_day + "/" + s_hour + ":" + s_min;
}

// operators
Date &Date::operator=(const Date &t_date) {
    // Date temp(t_date.getYear(), t_date.getMonth(), 
    //           t_date.getDay(), t_date.getHour(), t_date.getMinute());
    // return temp;
    m_year = t_date.getYear();
    m_month = t_date.getMonth();
    m_day = t_date.getDay();
    m_hour = t_date.getHour();
    m_minute = t_date.getMinute();
    return *this;
}

bool Date::operator==(const Date &t_date) const {
    return m_year == t_date.getYear()
         && m_month == t_date.getMonth()
         && m_day == t_date.getDay()
         && m_hour == t_date.getHour()
         && m_minute == t_date.getMinute();
}

bool Date::operator>=(const Date &t_date) const {
    return !((*this) < t_date);
}

bool Date::operator<=(const Date &t_date) const {
    return !((*this) > t_date);
}

bool Date::operator>(const Date &t_date) const {
    if (m_year > t_date.getYear()) return true;
    else if (m_year == t_date.getYear()){
        if (m_month > t_date.getMonth()) return true;
        else if (m_month == t_date.getMonth()) {
            if(m_day > t_date.getDay()) return true;
            else if (m_day == t_date.getDay()) {
                if (m_hour > t_date.getHour()) return true;
                else if (m_hour == t_date.getHour()) {
                    if (m_minute > t_date.getMinute()) return true;
                }
            }
        }
    }
    return false;
}

bool Date::operator<(const Date &t_date) const {
    if (m_year < t_date.getYear()) return true;
    else if (m_year == t_date.getYear()){
        if (m_month < t_date.getMonth()) return true;
        else if (m_month == t_date.getMonth()) {
            if(m_day < t_date.getDay()) return true;
            else if (m_day == t_date.getDay()) {
                if (m_hour < t_date.getHour()) return true;
                else if (m_hour == t_date.getHour()) {
                    if (m_minute < t_date.getMinute()) return true;
                }
            }
        }
    }
    return false;
}