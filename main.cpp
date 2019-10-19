#include <iostream>
#include "Date.hpp"
#include "Storage.hpp"
#include "Meeting.hpp"
#include "User.hpp"
#include "AgendaService.hpp"
#include "AgendaUI.hpp"
#include <string>
#include <vector>
using namespace std;

void unlogin();
vector<string> split(const string tar);

int main() {
    AgendaUI test;
}

// int main() {
//     AgendaService test;
//     unlogin();
//     string cmd;
//     // cin >> cmd;
//     while (cmd != "q") {
//         cout << "Agenda :~$ ";
//         cin >> cmd;
//         // system("clear");
//         if (cmd == "r") { // user register
//             string userName, password, email, phone;
//             cout << "Input your username: ";
//             cin >> userName;
//             cout << "Input your password: ";
//             cin >> password;
//             cout << "Input your email: ";
//             cin >> email;
//             cout << "Input your phone number: ";
//             cin >> phone;
//             if (test.userRegister(userName, password, email, phone))
//                 cout << "Create Success" << endl;
//             else cout << "Create Failed" << endl;
//         }
//         else if (cmd == "l") { // user log in
//             string userName, password, tar;
//             cout << "[log in] [username] [password]" << endl;
//             cout << "[log in] ";
//             cin >> tar;
//             vector<string> syn = split(tar);
//             if (syn.size() != 2 || !test.userLogIn(syn[0], syn[1])) 
//                 cout << "[log in] " << "Password error or user doesn't exist" << endl << endl;
//             else {
//                 cout << "[log in] succeed!" << endl << endl;
//                 string ucmd;
//                 while (ucmd != "o") {
//                     cin >> ucmd;
//                     if (ucmd == "cm") { // create meeting

//                     }
//                 }
//             }
//             // else cout << "[log in] " << "failed" << endl << endl;
//         }
//     }
//     return 0;
// }

// void unlogin() {
//     for(int i = 0; i < 27; i++) cout << '-';
//     cout << "Agenda";    
//     for(int i = 0; i < 27; i++) cout << '-';
//     cout << endl;
//     cout << "Action :" << endl;
//     cout << "l\t- log in Agenda by user name and password" << endl;
//     cout << "r\t- register an Agenda accout" << endl;
//     cout << "q\t- quit Agenda" << endl;
//     for(int i = 0; i < 60; i++) cout << '-';
//     cout << endl << endl;
// }

// vector<string> split(const string tar) {
//     vector<string> res;
//     int start = 0;
//     for(int i = 0; i < tar.length(); i++) {
//         if (tar[i] == ' ') {
//             res.push_back(tar.substr(start, i - start - 1));
//             start = i + 1;
//         }
//     }
//     res.push_back(tar.substr(start, tar.length() - start - 1));
//     return res;
// }
