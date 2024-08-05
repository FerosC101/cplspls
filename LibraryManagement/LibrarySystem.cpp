#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <ctime>
#include <limits>

using namespace std;

struct Book {
    string title;
    string author;
    bool isAvailable;

    Book(string t, string a) : title(t), author(a), isAvailable(true) {}

    void display() {
        cout << title << " by " << author << " - " << (isAvailable ? "Available" : "Issued") << endl;
    }
};

struct InventoryNode {
    Book* book;
    InventoryNode* next;

    InventoryNode(Book* b) : book(b), next(nullptr) {}
};

class Inventory {
private:
    InventoryNode* head;

public:
    Inventory() : head(nullptr) {}

    ~Inventory() {
        clear_inventory();
    }

    void add_book(Book* book) {
        InventoryNode* newNode = new InventoryNode(book);
        if (!head) {
            head = newNode;
        } else {
            InventoryNode* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void view_inventory() const {
        InventoryNode* current = head;
        while (current) {
            current->book->display();
            current = current->next;
        }
    }

    Book* search_book(const string& title) {
        InventoryNode* current = head;
        while (current) {
            if (current->book->title == title) {
                return current->book;
            }
            current = current->next;
        }
        return nullptr;
    }

    void clear_inventory() {
        while (head) {
            InventoryNode* temp = head;
            head = head->next;
            delete temp->book;
            delete temp;
        }
    }

    void load_books_from_file(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string title, author;
                getline(iss, title, ',');
                getline(iss, author);
                add_book(new Book(title, author));
            }
            file.close();
        } else {
            cout << "Unable to open books file." << endl;
        }
    }
};

struct BorrowRecord {
    string bookTitle;
    string date;

    BorrowRecord(const string& item, const string& d) : bookTitle(item), date(d) {}
};

struct User {
    string username;
    string password;
    vector<BorrowRecord> borrowHistory;

    User() : username(""), password("") {}
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
                iss >> user.username >> user.password;
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
                     << pair.second.password << "\n";
            }
            file.close();
        }
    }

    void save_borrow_history(const string& username) {
        ofstream file("borrowed_history.txt", ios::trunc);
        if (file.is_open()) {
            for (const auto& record : users[username].borrowHistory) {
                file << record.bookTitle << " " << record.date << "\n";
            }
            file.close();
        } 
    }

    void load_borrow_history(const string& username) {
        ifstream file("borrowed_history.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string bookTitle;
                string date;

                getline(iss, bookTitle, ' ');
                getline(iss, date);

                users[username].borrowHistory.emplace_back(bookTitle, date);
            }
            file.close();
        }
    }

public: 
    UserManager() {
        load_users();
    }

    ~UserManager() {
        save_users();
        for (const auto& pair : users) {
            save_borrow_history(pair.first);
        }
    }

    bool add_user(const string& username, const string& password) {
        if (users.find(username) != users.end()) {
            return false;
        }
        User newUser;
        newUser.username = username;
        newUser.password = password;
        users[username] = newUser;
        return true;
    }

    bool authenticate_user(const string& username, const string& password) {
        if (users.find(username) != users.end() && users[username].password == password) {
            return true;
        }
        return false;
    }

    User& get_user(const string& username) {
        return users[username];
    }
};

class Library {
private:
    Inventory inventory;
    UserManager userManager;
    queue<Book*> issueQueue;

    string get_current_date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        string date = to_string(ltm->tm_mday) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(1900 + ltm->tm_year);
        return date;
    }

public:
    void load_books(const string& filename) {
        inventory.load_books_from_file(filename);
    }

    void add_book(const string& title, const string& author) {
        Book* newBook = new Book(title, author);
        inventory.add_book(newBook);
    }

    void view_books() {
        inventory.view_inventory();
    }

    void issue_book(const string& username, const string& title) {
        Book* book = inventory.search_book(title);
        if (book && book->isAvailable) {
            book->isAvailable = false;
            issueQueue.push(book);
            string date = get_current_date();
            BorrowRecord record(title, date);
            userManager.get_user(username).borrowHistory.push_back(record);
            cout << "Book issued: " << title << endl;
        } else {
            cout << "Book is not available or does not exist" << endl;
        }
    }

    void return_book(const string& username, const string& title) {
        if (!issueQueue.empty()) {
            Book* book = issueQueue.front();
            if (book->title == title) {
                book->isAvailable = true;
                issueQueue.pop();
                cout << "Book returned: " << title << endl;
            } else {
                cout << "The book at the front of the queue does not match the title" << endl;
            }
        } else {
            cout << "No books are currently issued" << endl;
        }
    }

    void view_borrow_history(const string& username) {
        const vector<BorrowRecord>& history = userManager.get_user(username).borrowHistory;
        for (const auto& record : history) {
            cout << "Book: " << record.bookTitle << ", Date: " << record.date << endl;
        }
    }

    bool login(const string& username, const string& password) {
        return userManager.authenticate_user(username, password);
    }

    void register_user(const string& username, const string& password) {
        if (userManager.add_user(username, password)) {
            cout << "User registered successfully!" << endl;
        } else {
            cout << "Username already exists!" << endl;
        }
    }
};

int main() {
    Library lib;
    UserManager userManager;

    lib.load_books("books.txt");

    string username, password;
    bool loggedIn = false;

    while (true) {
        int choice;
        cout << "\nLibrary Management System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                lib.register_user(username, password);
                break;
            case 2:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (lib.login(username, password)) {
                    cout << "Login successful!" << endl;
                    loggedIn = true;

                    while (loggedIn) {
                        cout << "\n1. Add Book\n";
                        cout << "2. View Books\n";
                        cout << "3. Issue Book\n";
                        cout << "4. Return Book\n";
                        cout << "5. View Borrow History\n";
                        cout << "6. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                            case 1: {
                                string title, author;
                                cout << "Enter book title: ";
                                cin.ignore();
                                getline(cin, title);
                                cout << "Enter book author: ";
                                getline(cin, author);
                                lib.add_book(title, author);
                                break;
                            }
                            case 2:
                                lib.view_books();
                                break;
                            case 3: {
                                string title;
                                cout << "Enter book title: ";
                                cin.ignore();
                                getline(cin, title);
                                lib.issue_book(username, title);
                                break;
                            }
                            case 4: {
                                string title;
                                cout << "Enter book title: ";
                                cin.ignore();
                                getline(cin, title);
                                lib.return_book(username, title);
                                break;
                            }
                            case 5:
                                lib.view_borrow_history(username);
                                break;
                            case 6:
                                loggedIn = false;
                                cout << "Logged out successfully!" << endl;
                                break;
                            default:
                                cout << "Invalid choice!" << endl;
                        }
                    }
                } else {
                    cout << "Invalid username or password!" << endl;
                }
                break;
            case 3:
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}
