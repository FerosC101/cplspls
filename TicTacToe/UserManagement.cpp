#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct User {
    string username;
    string password;
    int wins;
    int losses;
};

class UserManager {
private:
    unordered_map<string, User> users;
    const string user_file = "users.txt";

    void load_users() {
        ifstream file(user_file);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                User user;
                iss >> user.username >> user.password >> user.wins >> user.losses;
                users[user.username] = user;
            }
            file.close();
        }
    }

    void save_users() {
        ofstream file(user_file);
        if (file.is_open()) {
            for (const auto& pair : users) {
                file << pair.second.username << " "
                     << pair.second.password << " "
                     << pair.second.wins << " "
                     << pair.second.losses << "\n";
            }
            file.close();
        }
    }

public:
    UserManager() {
        load_users();
    }

    bool register_user(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            return false;
        }
        users[username] = {username, password, 0, 0};
        save_users();
        return true;
    }

    bool login(const string& username, const string& password) {
        if (users.find(username) != users.end() && users[username].password == password) {
            return true;
        }
        return false;
    }

    void update_record(const string& username, bool win) {
        if (win) {
            users[username].wins++;
        } else {
            users[username].losses++;
        }
        save_users();
    }

    void get_record(const string& username) {
        if (users.find(username) != users.end()) {
            cout << "Wins: " << users[username].wins
                      << ", Losses: " << users[username].losses << endl;
        } else {
            cout << "User not found." << endl;
        }
    }
};
