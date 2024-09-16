#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
using namespace std;

struct Book {
    int id;
    string title;
    string author;
    bool isIssued;
    Book* next;
};

class BookList {
private:
    Book* head;

public:
    BookList() : head(nullptr) {}

    void addBook(int id, string title, string author, bool isIssued = false) {
        Book* newBook = new Book{id, title, author, isIssued, nullptr};
        if (!head) {
            head = newBook;
        } else {
            Book* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newBook;
        }
        cout << "Book added successfully!" << endl;
    }

    Book* searchBook(string title) {
        Book* temp = head;
        while (temp) {
            if (temp->title == title) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void removeBook(int id) {
        if (!head) return;

        if (head->id == id) {
            Book* temp = head;
            head = head->next;
            delete temp;
            cout << "Book removed successfully!" << endl;
            return;
        }

        Book* temp = head;
        while (temp->next && temp->next->id != id) {
            temp = temp->next;
        }

        if (temp->next) {
            Book* toDelete = temp->next;
            temp->next = toDelete->next;
            delete toDelete;
            cout << "Book removed successfully!" << endl;
        }
    }

    void displayBooks() {
        Book* temp = head;
        if (!temp) {
            cout << "No books available in the library." << endl;
            return;
        }
        while (temp) {
            cout << "ID: " << temp->id << ", Title: " << temp->title << ", Author: " << temp->author
                 << ", Issued: " << (temp->isIssued ? "Yes" : "No") << endl;
            temp = temp->next;
        }
    }

    bool issueBook(string title) {
        Book* book = searchBook(title);
        if (book && !book->isIssued) {
            book->isIssued = true;
            cout << "Book \"" << title << "\" has been issued." << endl;
            return true;
        } else if (book && book->isIssued) {
            cout << "Book \"" << title << "\" is already issued." << endl;
        } else {
            cout << "Book \"" << title << "\" not found." << endl;
        }
        return false;
    }

    void returnBook(string title) {
        Book* book = searchBook(title);
        if (book && book->isIssued) {
            book->isIssued = false;
            cout << "Book \"" << title << "\" has been returned." << endl;
        } else if (book && !book->isIssued) {
            cout << "Book \"" << title << "\" was not issued." << endl;
        } else {
            cout << "Book \"" << title << "\" not found." << endl;
        }
    }

    void saveToCSV(string filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Error: Could not open file for writing." << endl;
            return;
        }
        Book* temp = head;
        while (temp) {
            file << temp->id << "," << temp->title << "," << temp->author << "," << (temp->isIssued ? "1" : "0") << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Books saved to file successfully!" << endl;
    }
    void loadFromCSV(string filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Error: Could not open file for reading." << endl;
            return;
        }
        string line, title, author;
        int id;
        bool isIssued;
        while (getline(file, line)) {
            stringstream ss(line);
            string issued;
            getline(ss, line, ',');
            id = stoi(line);
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, issued);
            isIssued = (issued == "1");
            addBook(id, title, author, isIssued);
        }
        file.close();
        cout << "Books loaded from file successfully!" << endl;
    }
};

void displayMenu() {
    cout << "\nLibrary Management System Menu" << endl;
    cout << "1. Display all books" << endl;
    cout << "2. Add a book" << endl;
    cout << "3. Remove a book" << endl;
    cout << "4. Search for a book" << endl;
    cout << "5. Issue a book" << endl;
    cout << "6. Return a book" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    BookList library;
    int choice, id;
    string title, author;
    string filename = "books.csv";

    library.loadFromCSV(filename);

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                library.displayBooks();
                break;
            case 2:
                cout << "Enter book ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter book author: ";
                getline(cin, author);
                library.addBook(id, title, author);
                break;
            case 3:
                cout << "Enter book ID to remove: ";
                cin >> id;
                library.removeBook(id);
                break;
            case 4:
                cin.ignore();  
                cout << "Enter book title to search: ";
                getline(cin, title);
                if (library.searchBook(title)) {
                    cout << "Book found!" << endl;
                } else {
                    cout << "Book not found." << endl;
                }
                break;
            case 5:
                cin.ignore();  
                cout << "Enter book title to issue: ";
                getline(cin, title);
                library.issueBook(title);
                break;
            case 6:
                cin.ignore();  
                cout << "Enter book title to return: ";
                getline(cin, title);
                library.returnBook(title);
                break;
            case 0:
                cout << "Saving books to file..." << endl;
                library.saveToCSV(filename);
                cout << "Exiting the system." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
