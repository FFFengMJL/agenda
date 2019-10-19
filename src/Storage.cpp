#include "Storage.hpp"
#include <fstream>
#include <iostream> // debug
#include "Path.hpp"
#include <vector>
#include <string>

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
    // if (m_instance == nullptr) m_instance = getInstance();
    bool a = readFromFile();
    m_dirty = false;
}

Storage::~Storage() {
    sync();
}

// cannot
bool Storage::writeToFile() {
    std::fstream ufile, mfile;
    ufile.open(Path::userPath, std::ios::out);
    mfile.open(Path::meetingPath, std::ios::out);

    // ufile.open("./users.csv", std::ios::out);
    // mfile.open("./meetings.csv", std::ios::out);

    if(!ufile.is_open() || !mfile.is_open()) return false;

    ufile.clear();
    mfile.clear();

    for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
        User temp = *i;
        std::string user = "\"" 
            + temp.getName() + "\",\""
            + temp.getPassword() + "\",\""
            + temp.getEmail() + "\",\""
            + temp.getPhone() + "\"\n";
        ufile << user;
    }

    for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
        Meeting temp = *i;
        std::vector<std::string> parts = temp.getParticipator();
        std::string p;
        for (auto index = parts.begin(); index != parts.end(); index++) {
            p += *index + '&';
        }
        p = p.substr(0, p.length() - 1);
        std::string meeting = "\""
            + temp.getSponsor() + "\",\""
            + p + "\",\""
            + Date::dateToString(temp.getStartDate()) + "\",\""
            + Date::dateToString(temp.getEndDate()) + "\",\""
            + temp.getTitle() + "\"\n";
        mfile << meeting;
    }
    mfile.close();
    ufile.close();
    m_dirty = false;

    return true;
}

bool Storage::readFromFile() {

    // user
    std::fstream ufile, mfile;
    ufile.open(Path::userPath, std::ios::in);
    mfile.open(Path::meetingPath, std::ios::in);

    // ufile.open("./users.csv", std::ios::in);
    // mfile.open("./meetings.csv", std::ios::in);

    if(!ufile.is_open() || !mfile.is_open()) return false;
    std::string user;
    while (!ufile.eof()) {
        getline(ufile, user);
        if(user.length() <= 2 || user == "\n") break;
        // std::cout << user << std::endl;
        std::vector<std::string> t_user;
        int start = 0, end = 0;
        for (int i = 0; i < user.length(); i++) {
            if (user[i] == ',') {
                end = i;
                t_user.push_back(user.substr(start + 1, end - start - 2));
                start = end + 1;
            }
        }
        t_user.push_back(user.substr(start + 1, user.length() -start - 2));
        // std::cout << t_user[0] << " ";
        // std::cout << t_user[1] << " ";
        // std::cout << t_user[2] << " ";
        // std::cout << t_user[3] << std::endl;
        createUser(User(t_user[0], t_user[1], t_user[2], t_user[3]));
        // std::cout << 1 << std::endl;
    }

    std::string meeting;
    while (!mfile.eof()) {
        getline(mfile, meeting);
        if(meeting.length() <= 2) break;
        // std::cout << meeting << std::endl;
        std::vector<std::string> t_meet;
        int start = 0, end = 0;
        for (int i = 0; i < meeting.length(); i++) {
            if (meeting[i] == ',') {
                end = i;
                t_meet.push_back(meeting.substr(start + 1, end - start - 2));
                start = i + 1;
            }
        }
        t_meet.push_back(meeting.substr(start + 1, meeting.length() - start - 2));
        std::vector<std::string> parts;
        std::string p = t_meet[1];
        // std::cout << t_meet[1];
        int start_p = 0, end_p = 0;
        for (int i = 0; i < p.length(); i++) {
            if (p[i] == '&') {
                end_p = i;
                parts.push_back(p.substr(start_p, end_p - start_p));
                // std::cout << p.substr(start_p, end_p - start_p) << std::endl;
                start_p = i + 1;
            }
        }
        // std::cout << 4 << std::endl;
        // std::cout << t_meet[0] << std::endl;
        // std::cout << parts[0] << std::endl;
        // std::cout << t_meet[2] << std::endl;
        // std::cout << t_meet[3] << std::endl;
        // std::cout << t_meet[4] << std::endl;
        parts.push_back(p.substr(start_p, p.length() - start_p));

        // for (auto i = parts.begin(); i != parts.end(); i++) {
        //     std::cout << *i << std::endl;
        // }
        // std::cout << parts[1] << std::endl;
        // std::cout << t_meet[4] << std::endl;
        // std::cout << 3 << std::endl;
        createMeeting(Meeting(t_meet[0],parts, Date(t_meet[2]), Date(t_meet[3]), t_meet[4]));
    }
    ufile.close();
    mfile.close();
    m_dirty = false;
    return true;

}

std::shared_ptr<Storage> Storage::getInstance() {
    if (m_instance == nullptr) m_instance = std::shared_ptr<Storage> (new Storage());
    return m_instance;
}

void Storage::createUser(const User &t_user) {
    m_dirty = true;
    m_userList.push_back(t_user);
    // std::cout << 1 << std::endl;
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
    std::list<User> temp;
    for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
        if (filter(*i)) temp.push_back(*i);
    }
    return temp;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                        std::function<void(User &)> switcher) {
    int res = 0;
    for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
        if (filter(*i)) {
            switcher(*i);
            res++;
        }
    }
    if(res != 0) m_dirty = true;
    return res;     
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int res = 0;
    for (auto i = m_userList.begin(); i != m_userList.end();) {
        if (filter(*i)) {
            auto tar = i;
            i = m_userList.erase(i);
            res++;
            continue;
        }i++;
    }
    if(res != 0) m_dirty = true;
    return res;
}

void Storage::createMeeting(const Meeting &t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
    // std::cout << 2 << std::endl;
}

std::list<Meeting> Storage::queryMeeting(
    std::function<bool(const Meeting &)> filter) const {
        std::list<Meeting> temp;
    for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
        if (filter(*i)) temp.push_back(*i);
    }
    return temp;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                std::function<void(Meeting &)> switcher) {
    int res = 0;
    for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
        if (filter(*i)) {
            switcher(*i);
            res++;
        }
    }
    if(res != 0) m_dirty = true;
    return res;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int res = 0;
    for (auto i = m_meetingList.begin(); i != m_meetingList.end(); ) {
        if (filter(*i)) {
            auto tar = i;
            i = m_meetingList.erase(i);
            res++;
            continue;
        }i++;
    }
    if(res != 0) m_dirty = true;
    return res;
}

bool Storage::sync() {
    if (m_dirty == false) return false;
    return writeToFile();
}



