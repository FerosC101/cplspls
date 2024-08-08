#include <iostream>
#include <mysql/mysql.h>
#include <vector>
#include <sstream>
#include <ctime>
#include <limits>
#include <string>

using namespace std;

MYSQL* connectDB() {
    MYSQL* conn = mysql_init(nullptr);
    if (conn) {
        if (mysql_real_connect(conn, "localhost", "root", "password", "library", 3306, nullptr, 0)) {
            cout << "Database connection successful!" << endl;
        } else {
            cout << "Database connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cout << "MySQL initialization failed" << endl;
    }
    return conn;
}

void closeDB(MYSQL* conn) {
    mysql_close(conn);
}

bool authenticate_user(MYSQL* conn, const string& username, const string& password) {
    MYSQL_RES* res;
    MYSQL_ROW row;
    string query = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
        return false;
    }
    
    res = mysql_store_result(conn);
    if ((row = mysql_fetch_row(res))) {
        mysql_free_result(res);
        return true;
    } else {
        mysql_free_result(res);
        return false;
    }
}

bool register_user(MYSQL* conn, const string& username, const string& password) {
    string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
        return false;
    }
    return true;
}

void add_book(MYSQL* conn, const string& title, const string& author) {
    string query = "INSERT INTO books (title, author, is_available) VALUES ('" + title + "', '" + author + "', true)";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
    } else {
        cout << "Book added successfully!" << endl;
    }
}

void view_books(MYSQL* conn) {
    MYSQL_RES* res;
    MYSQL_ROW row;
    string query = "SELECT * FROM books";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
        return;
    }
    
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        cout << row[1] << " by " << row[2] << " - " << (atoi(row[3]) ? "Available" : "Issued") << endl;
    }
    mysql_free_result(res);
}

void issue_book(MYSQL* conn, const string& username, const string& title) {
    MYSQL_RES* res;
    MYSQL_ROW row;
    string query = "SELECT * FROM books WHERE title = '" + title + "' AND is_available = true";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
        return;
    }
    
    res = mysql_store_result(conn);
    if ((row = mysql_fetch_row(res))) {
        int book_id = atoi(row[0]);
        mysql_free_result(res);

        query = "UPDATE books SET is_available = false WHERE id = " + to_string(book_id);
        if (mysql_query(conn, query.c_str())) {
            cout << "MySQL query error: " << mysql_error(conn) << endl;
            return;
        }

        query = "SELECT id FROM users WHERE username = '" + username + "'";
        if (mysql_query(conn, query.c_str())) {
            cout << "MySQL query error: " << mysql_error(conn) << endl;
            return;
        }
        
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        int user_id = atoi(row[0]);
        mysql_free_result(res);

        string date = get_current_date();
        query = "INSERT INTO borrow_history (user_id, book_id, borrow_date) VALUES (" + to_string(user_id) + ", " + to_string(book_id) + ", '" + date + "')";
        if (mysql_query(conn, query.c_str())) {
            cout << "MySQL query error: " << mysql_error(conn) << endl;
        } else {
            cout << "Book issued: " << title << endl;
        }
    } else {
        cout << "Book is not available or does not exist" << endl;
        mysql_free_result(res);
    }
}

void return_book(MYSQL* conn, const string& username, const string& title) {
    MYSQL_RES* res;
    MYSQL_ROW row;
    string query = "SELECT * FROM books WHERE title = '" + title + "' AND is_available = false";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
        return;
    }
    
    res = mysql_store_result(conn);
    if ((row = mysql_fetch_row(res))) {
        int book_id = atoi(row[0]);
        mysql_free_result(res);

        query = "UPDATE books SET is_available = true WHERE id = " + to_string(book_id);
        if (mysql_query(conn, query.c_str())) {
            cout << "MySQL query error: " << mysql_error(conn) << endl;
            return;
        }

        query = "DELETE FROM borrow_history WHERE book_id = " + to_string(book_id) + " ORDER BY borrow_date DESC LIMIT 1";
        if (mysql_query(conn, query.c_str())) {
            cout << "MySQL query error: " << mysql_error(conn) << endl;
        } else {
            cout << "Book returned: " << title << endl;
        }
    } else {
        cout << "No record found for the issued book" << endl;
        mysql_free_result(res);
    }
}

void view_borrow_history(MYSQL* conn, const string& username) {
    MYSQL_RES* res;
    MYSQL_ROW row;
    string query = "SELECT id FROM users WHERE username = '" + username + "'";
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
        return;
    }
    
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    int user_id = atoi(row[0]);
    mysql_free_result(res);

    query = "SELECT books.title, borrow_history.borrow_date FROM borrow_history JOIN books ON borrow_history.book_id = books.id WHERE borrow_history.user_id = " + to_string(user_id);
    
    if (mysql_query(conn, query.c_str())) {
        cout << "MySQL query error: " << mysql_error(conn) << endl;
        return;
    }
    
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res))) {
        cout << "Book: " << row[0] << ", Date: " << row[1] << endl;
    }
    mysql_free_result(res);
}

string get_current_date() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string date = to_string(ltm->tm_mday) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(1900 + ltm->tm_year);
    return date;
}

int main() {
    MYSQL* conn = connectDB();
    if (!conn) return 1;

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
                if (register_user(conn, username, password)) {
                    cout << "Registration successful!" << endl;
                } else {
                    cout << "Registration failed!" << endl;
                }
                break;
            case 2:
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (authenticate_user(conn, username, password)) {
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
                                add_book(conn, title, author);
                                break;
                            }
                            case 2:
                                view_books(conn);
                                break;
                            case 3: {
                                string title;
                                cout << "Enter book title: ";
                                cin.ignore();
                                getline(cin, title);
                                issue_book(conn, username, title);
                                break;
                            }
                            case 4: {
                                string title;
                                cout << "Enter book title: ";
                                cin.ignore();
                                getline(cin, title);
                                return_book(conn, username, title);
                                break;
                            }
                            case 5:
                                view_borrow_history(conn, username);
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
                closeDB(conn);
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}
