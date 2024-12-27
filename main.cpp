
#include <bits/stdc++.h>
#include <cstdio>
#include <ctime>
#include <thread>
#include <windows.h>
#include "sqlite3.h"

#define intx int64_t
using namespace std;

sqlite3 *db;
sqlite3_stmt *stmt;
int result;
string query;

void menubar();
void nextbus();
void login();
void logout();
void terminatex();
int curtime();
void addnewbus();
void removebus();
int logincheck(string username, int password);
int isUserRegistered();
void registerUser();

int main() {

    cout<<endl;
    cout<<" {__BUS TIMING MANAGEMENT MBSTU__}"<<endl<<endl;
    result = sqlite3_open("bus_timing_info.db", &db);
    if (result != SQLITE_OK) {
        cout << "Hmm1..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
        return 1;
    }

    query = "CREATE TABLE IF NOT EXISTS bus(time INT, origin VARCHAR(50), destination VARCHAR(50));";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cout << "Hmm2..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
    }
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    query = "CREATE TABLE IF NOT EXISTS logininfo(username VARCHAR(50), password INT);";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cout << "Hmm4..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
    }
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    while (1) {
        menubar();
    }
}

void menubar() {
    cout << "   ---------------------------" << endl;
    cout << " \033[42m|             MENU           |\033[0m" << endl;
    cout << "   ---------------------------" << endl;
    cout << "   |   Select Operation->     |" << endl;
    cout << "   |   [1] Watch Next Bus     |" << endl;
    cout << "   |   [2] Login              |" << endl;
    cout << "   |   [3] Logout             |" << endl;
    cout << "   |   [4] Terminate Program  |" << endl;
    cout << "   ---------------------------" << endl;

    int operation;
    cin >> operation;
    getchar();

     system("cls");
    switch (operation) {
        case 1:
            nextbus();
            break;
        case 2:
            login();
            break;
        case 3:
            logout();
            break;
        case 4:
            terminatex();
            break;
        default:
            for (int i = 0; i < 15; i++) {
                cout << "\033[41m. \033[0m";
                Sleep(200);
            }
            getchar();
            cout << "\n\033[31mInvalid choice...Try again.\033[0m" << endl;
            menubar();
            break;
    }
}

void nextbus() {
    int now = curtime();
    const char* query = "SELECT * FROM bus WHERE time >= ?;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cout << "Hmm..We got some glitch at -> " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, now);
    cout << "  ---------------------------" << endl;
    cout << "    \033[46m";
    cout << "        NEXT BUSES     \033[0m" << endl;

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        int x = sqlite3_column_int(stmt, 0);
        string s = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string ss = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        int hour = x / 100;
        int minute = x % 100;
        string period = "AM";
        if (hour >= 12) {
            period = "PM";
            if (hour > 12) hour -= 12;
        } else if (hour == 0) {
            hour = 12;
        }

        cout << "\033[92m   ---------------------------\033[0m" << endl;
        cout << "     TIME       : " << hour << ":" << setw(2) << setfill('0') << minute << " " << period << endl;
        cout << "     DEPARTURE  : " << s << endl;
        cout << "     DESTINATION: " << ss << endl;
        cout << "\033[92m   ---------------------------\033[0m" << endl << endl;
        Sleep(100);
    }

    if (result != SQLITE_DONE) {
        cout << "Hmm..We got some glitch at -> " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

void login() {
    if (!isUserRegistered()) {
        registerUser();
    }
    cout << "  " << "\033[42m   Login to Admin    \033[0m" << endl;
    string username;
    int password;
    cout << "  Enter username ->\033[33m";
    //cin.ignore();
    getline(cin, username); cout << "\033[0m";
    cout << "  Enter Password ->  \033[33m";
    cin >> password; cout << "\033[0m";

    if (logincheck(username, password)) {
        cout << "\n  ";
        for (int i = 0; i < 16; i++) {
            cout << "\033[100m  ";
            Sleep(100);
        }cout << "\033[0m";
        cout << endl;// cout << "\033[100m";//cout << "\033[100m";


        cout << "\033[103m\033[30m    -------------------------------\033[0m\033[0m" << endl;
        cout << "\033[103m\033[30m           Admin Panel            \033[0m\033[0m" << endl;
        cout << "\033[103m\033[30m    -------------------------------\033[0m\033[0m" << endl;
        cout << "\033[103m\033[30m    |  [1] Add new schedule       |\033[0m\033[0m" << endl;
        cout << "\033[103m\033[30m    |  [2] Remove a schedule      |\033[0m\033[0m" << endl;
        cout << "\033[103m\033[30m    |  [3] EXIT Login             |\033[0m\033[0m" << endl;
        cout << "\033[103m\033[30m    -------------------------------\033[0m\033[0m\n" << endl;

        int choice;
        cin >> choice;
        cin.ignore();  // Clear the buffer after input
        switch (choice) {
            case 1:
                addnewbus();
                break;
            case 2:
                removebus();
                break;
            case 3:
                cout << "\033[91m  EXITING LOGIN.....\033[0m" << endl;
                cout << "  ";
                for (int i = 0; i < 15; i++) {
                    cout << "\033[104m  \033[0m";
                    Sleep(200);
                }
                cout << endl;
                menubar();
                break;
            default:
                cout << "Invalid choice...Try again." << endl;
                menubar();
        }
    } else {
        cout << "Username or Password is wrong" << endl;
        menubar();
    }
}

void logout() {

   string user;
   int pass;
   cout<<"Enter your current Username->"<<endl;
   cin>>user;
   cout<<"Enter your current password->"<<endl;
   cin>>pass;
   getchar();
   query="DELETE from logininfo";
   result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
   sqlite3_step(stmt);
   sqlite3_finalize(stmt);



   //Update needs here
    menubar();
}

void terminatex() {
    cout << "\nProgram is Exiting........" << endl;
    for (int i = 0; i < 15; i++) {
        cout << "\033[41m. \033[0m";
        Sleep(200);
    }
    cout << "\n...........TERMINATED........." << endl;
    exit(0);
}

int curtime() {
    time_t current_time = time(0);
    struct tm* local_time = localtime(&current_time);
    int current_time_int = (local_time->tm_hour * 100) + local_time->tm_min;
    return current_time_int;
}

void addnewbus() {
    int time;
    string origin;
    string destination;
    cout << "Enter Departure Time-> ";
    cin >> time;
    getchar();
    cout << "Enter Departure Location-> ";
    getline(cin, origin);
    cout << "Enter Destination Location-> ";
    getline(cin, destination);

    query = "INSERT INTO bus(time, origin, destination) VALUES(?, ?, ?)";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cout << "Hmm3..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
    }

    sqlite3_bind_int(stmt, 1, time);
    sqlite3_bind_text(stmt, 2, origin.c_str(), origin.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, destination.c_str(), destination.length(), SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "\033[93mNew bus added successfully!\033[0m" << endl;
}

void removebus() {
    int time;
    cout << "Enter Departure Time to remove-> ";
    cin >> time;
    getchar();

    query = "DELETE FROM bus WHERE time = ?";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cout << "Hmm3..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
    }

    sqlite3_bind_int(stmt, 1, time);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "\033[93mBus removed successfully!\033[0m" << endl;
}

int logincheck(string username, int password) {
    query = "SELECT * FROM logininfo WHERE username = ? AND password = ?";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cout << "Hmm6..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), username.length(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, password);

    int check = 0;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        check = 1;
    }

    sqlite3_finalize(stmt);
    return check;
}

int isUserRegistered() {
    query = "SELECT * FROM logininfo LIMIT 1";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cout << "Hmm5..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
    }

    int registered = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        registered = 1;
    }

    sqlite3_finalize(stmt);
    return registered;
}

void registerUser() {
    string username;
    int password;
    cout << "Create an admin username -> ";
    getline(cin, username);
    cout << "Create a password -> ";
    cin >> password;
    getchar();

    query = "INSERT INTO logininfo(username, password) VALUES(?, ?)";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        cout << "Hmm5..We got some glitch at ->" << sqlite3_errmsg(db) << endl;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), username.length(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, password);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    cout << "Admin created successfully." << endl;
}
