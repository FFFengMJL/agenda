#include "AgendaUI.hpp"
#include <iostream>
using namespace std;

vector<string> split(const string tar) {
    vector<string> res;
    int start = 0;
    for(int i = 0; i < tar.length(); i++) {
        if (tar[i] == ' ') {
            res.push_back(tar.substr(start, i - start - 1));
            start = i + 1;
        }
    }
    res.push_back(tar.substr(start, tar.length() - start - 2));
    return res;
}


AgendaUI::AgendaUI() {
    startAgenda();
    // quitAgenda();
}

// AgendaUI::~AgendaUI() {
//     quitAgenda();
// }

void AgendaUI::startAgenda() {
    for(int i = 0; i < 27; i++) std::cout << '-';
    std::cout << "Agenda";    
    for(int i = 0; i < 27; i++) std::cout << '-';
    std::cout << std::endl;
    std::cout << "Action :" << std::endl;
    std::cout << "l\t- log in Agenda by user name and password" << std::endl;
    std::cout << "r\t- register an Agenda acstd::cout" << std::endl;
    std::cout << "q\t- quit Agenda" << std::endl;
    for(int i = 0; i < 60; i++) std::cout << '-';
    std::cout << std::endl << std::endl;
    OperationLoop();
}

void AgendaUI::OperationLoop(void) {
    std::string cmd;
    while (cmd != "q") {
        cmd = getOperation();
        if (m_userName.length() == 0) {
            if (cmd == "r") userRegister();
            else if (cmd == "l") userLogIn();
        }
        else executeOperation(cmd);
    }
    quitAgenda();
}

std::string AgendaUI::getOperation() {
    if (m_userName.length() == 0) cout << "Agenda :~$ ";
    else cout << "Agenda@" << m_userName << " :~# ";
    // cout << m_userName << endl;
    std::string cmd;
    std::cin >> cmd;
    return cmd;
}

bool AgendaUI::executeOperation(std::string t_operation) {
    if (t_operation == "o") userLogOut();
    else if (t_operation == "lu") listAllUsers();
    else if (t_operation == "cm") createMeeting();
    else if (t_operation == "la") listAllMeetings();
    else if (t_operation == "las") listAllSponsorMeetings();
    else if (t_operation == "lap") listAllParticipateMeetings();
    else if (t_operation == "qm") queryMeetingByTitle();
    else if (t_operation == "qt") queryMeetingByTimeInterval();
    else if (t_operation == "dm") deleteMeetingByTitle();
    else if (t_operation == "da") deleteAllMeetings();
    else if (t_operation == "dc") deleteUser();
    // else if (t_operation == "q") break;
    // need "rqm" "amp" "rmp"
}

/**
 * user Login
 */
void AgendaUI::userLogIn(void) {
    string tar;
    cout << endl << "[log in] [user name] [password]" << endl;
    cout << "[log in] ";
    // cout << tar << endl;
    vector<string> user;
    for (int i = 0; i < 2 ; i++) {
        cin >> tar;
        user.push_back(tar);
        // cout << tar;
    }
    // cout << user.size() << endl;
    if (m_agendaService.userLogIn(user[0], user[1])) {
        m_userName = user[0];
        m_userPassword = user[1];
        cout << "[log in] succeed!" << endl << endl;
        for(int i = 0; i < 27; i++) std::cout << '-';
        std::cout << "Agenda";    
        for(int i = 0; i < 27; i++) std::cout << '-';
        std::cout << std::endl;
        std::cout << "Action :" << std::endl;
        cout << "o\t- log out Agenda" << endl;
        cout << "dc\t- delete Agenda account" << endl;
        cout << "lu\t- list all Agenda user" << endl;
        cout << "cm\t- create a meeting" << endl;
        cout << "amp\t- add meeting participator" << endl;
        cout << "rmp\t- remove meeting participator" << endl;
        cout << "rqm\t- request to quit meeting" << endl;
        cout << "la\t- list all meetings" << endl;
        cout << "las\t- list all sponsor meetings" << endl;
        cout << "lap\t- list all participator meetings" << endl;
        cout << "qm\t- query meeting by title" << endl;
        cout << "qt\t- query meeting by time interval" << endl;
        cout << "dm\t- delete meeting by title" << endl;
        cout << "da\t- delete all meetings" << endl;
        for (int i = 0; i < 60; i++) cout << '-';
        cout << endl << endl;
    }
    else {
        cout << "[log in] Password error or user doesn't exist" << endl << endl;
    }
}

/**
 * user regist
 */
void AgendaUI::userRegister(void) {
    string tar;
    vector<string> user;
    cout << endl << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    for (int i = 0; i < 4; i++) {
        cin >> tar;
        user.push_back(tar);
        // cout << tar << endl;
    }
    if (m_agendaService.userRegister(user[0], user[1], user[2], user[3])) 
        cout << "[register] succeed!" << endl;
    else cout << "[register] This username has been registered" << endl; 
}

/**
 * user logout
 */
void AgendaUI::userLogOut(void) {
    m_userName = "";
    m_userPassword = "";
    std::cout << std::endl;
    std::cout << "Action :" << std::endl;
    std::cout << "l\t- log in Agenda by user name and password" << std::endl;
    std::cout << "r\t- register an Agenda acstd::cout" << std::endl;
    std::cout << "q\t- quit Agenda" << std::endl;
    for(int i = 0; i < 60; i++) std::cout << '-';
    std::cout << std::endl << std::endl;

}

/**
 * quit the Agenda
 */
void AgendaUI::quitAgenda(void) {
    m_agendaService.quitAgenda();
}

/**
 * delete a user from storage
 */
void AgendaUI::deleteUser(void) {
    if (m_agendaService.deleteUser(m_userName, m_userPassword))
        cout << "[delete agenda account] succeed!" << endl << endl;
    else cout << "[delete agenda account] failed!" << endl << endl;
}

/**
 * list all users from storage
 */
void AgendaUI::listAllUsers(void) {
    std::list<User> user = m_agendaService.listAllUsers();
    std:: cout << "[list all users]" << std::endl << std::endl;
    for (auto i = user.begin(); i != user.end(); i++) {
        std::cout << i->getName() << '\t' << 
                     i->getEmail() << '\t' << 
                     i->getPhone() << std::endl;
    }
}

/**
 * user create a meeting with someone else
 */
void AgendaUI::createMeeting(void) {
    string tar;
    int num;
    vector<string> meeting;
    vector<string> participator;
    cout << "[create meeting] [the number of participators]";
    cout << "[create meeting] ";
    cin >> tar;
    for (int i = 0; i< num; i++) {
        cout << "[create meeting] [please enter the participator " << i << " ]" << endl;
        cin >> tar;
        participator.push_back(tar);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting]";
    for (int i = 0; i< 4; i++) {
        cin >> tar;
        meeting.push_back(tar);
    }
    if (m_agendaService.createMeeting(meeting[0], meeting[1], meeting[2], meeting[3], participator))
        cout << "[create meeting] succeed!";
    else cout << "[create meeting] error";
}

/**
 * list all meetings from storage
 */
void AgendaUI::listAllMeetings(void) {
    list<Meeting> meetings = m_agendaService.listAllMeetings(m_userName);
    printMeetings(meetings);
}

/**
 * list all meetings that this user sponsored
 */
void AgendaUI::listAllSponsorMeetings(void) {}

/**
 * list all meetings that this user take part in
 */
void AgendaUI::listAllParticipateMeetings(void) {}

/**
 * search meetings by title from storage
 */
void AgendaUI::queryMeetingByTitle(void) {
    string title;
    cout << "[query meeting] [title]:" << endl;
    cout << "[query meeting] ";
    cin >> title;
    printMeetings(m_agendaService.meetingQuery(m_userName, title));
}

/**
 * search meetings by timeinterval from storage
 */
void AgendaUI::queryMeetingByTimeInterval(void) {
    string startTime, endTime;
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting]";
    cin >> startTime >> endTime;
    printMeetings(m_agendaService.meetingQuery(m_userName, startTime, endTime));
}

/**
 * delete meetings by title from storage
 */
void AgendaUI::deleteMeetingByTitle(void) {
    string title;
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting]";
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title)) cout << "[delete meeting] succeed!" << endl;
    else cout << "[error] delete meeting fail!" << endl;
}

/**
 * delete all meetings that this user sponsored
 */
void AgendaUI::deleteAllMeetings(void) {
    if (m_agendaService.deleteAllMeetings(m_userName)) cout << "[delete all meeting] succeed!";
    else cout << "[error] delete all meeting fail!" << endl;
}

/**
 * show the meetings in the screen
 */
void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    cout << "title\tsponsor\tstart time\tedn time\tparticipator\n";
    for (auto i = t_meetings.begin(); i != t_meetings.end(); i++) {
        cout << i->getTitle() << '\t' << i->getSponsor() << '\t';
        cout << Date::dateToString(i->getStartDate()) << '\t' <<
                Date::dateToString(i->getEndDate()) << '\t';
        string pear;
        for (auto t = i->getParticipator().begin(); t != i->getParticipator().end(); t++) {
            pear = pear + *t + ',';
        }
        pear = pear.substr(0, pear.length() - 1);
        cout << pear << endl;
    }
}
