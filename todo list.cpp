#include <iostream>
#include <queue>
#include <string>

using namespace std;

void displayTodoList(queue<string> &todoQueue) {
    if (todoQueue.empty()) {
        cout << "To do list is Empty." << endl;
    } else {
        cout << "To do List:" << endl;
        int count = 1;
        while (!todoQueue.empty()) {
            cout << count << ". " << todoQueue.front() << endl;
            todoQueue.pop();
            count++;
        }
    }
}

int main() {
    queue<string> todoQueue;

    while (true) {
        cout << "\nTo do List Menu:" << endl;
        cout << "1. Add to do\n2. Remove To do\n3. Display To do List\n4. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter to do item: ";
                string todoItem;
                cin.ignore();
                getline(cin, todoItem);
                todoQueue.push(todoItem);
                cout << "Added Successfully." << endl;
                break;
            }
            case 2: {
                if (todoQueue.empty()) {
                    cout << "To do list is empty. There's nothing to remove." << endl;
                } else {
                    cout << "Removing Item: " << todoQueue.front() << endl;
                    todoQueue.pop();
                }
                break;
            }
            case 3:
                displayTodoList(todoQueue);
                break;
            case 4:
                cout << "Exiting Program" << endl;
                return 0;
            default:
                cout << "Invalid Choice, Choose again" << endl;
        }
    }
}