#include <iostream>
#include <limits>

using namespace std;

struct Node {
    int data;
    Node* next;
};

class Stack {
public:
    Stack() : top(nullptr) {}

    void push(int data);
    void pop();
    int peek();
    bool isEmpty();
    void display();

private:
    Node* top;
};

void Stack::push(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = top;
    top = newNode;
}

void Stack::pop() {
    if (top == nullptr) {
        cout << "Stack is empty. Cannot pop." << endl;
        return;
    }

    Node* temp = top;
    top = top->next;
    delete temp;
}

int Stack::peek() {
    if (top == nullptr) {
        cout << "Stack is empty." << endl;
        return -1;
    }
    return top->data;
}

bool Stack::isEmpty() {
    return top == nullptr;
}

void Stack::display() {
    Node* temp = top;
    while (temp != nullptr) {
        cout << temp->data << " -> ";
        temp = temp->next;
    }
    cout << "nullptr" << endl;
}

class Queue {
public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(int data);
    void dequeue();
    int peek();
    bool isEmpty();
    void display();

private:
    Node* front;
    Node* rear;
};

void Queue::enqueue(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = nullptr;

    if (rear == nullptr) {
        front = rear = newNode;
        return;
    }

    rear->next = newNode;
    rear = newNode;
}

void Queue::dequeue() {
    if (front == nullptr) {
        cout << "Queue is empty. Cannot dequeue." << endl;
        return;
    }

    Node* temp = front;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
}

int Queue::peek() {
    if (front == nullptr) {
        cout << "Queue is empty." << endl;
        return -1;
    }
    return front->data;
}

bool Queue::isEmpty() {
    return front == nullptr;
}

void Queue::display() {
    Node* temp = front;
    while (temp != nullptr) {
        cout << temp->data << " -> ";
        temp = temp->next;
    }
    cout << "nullptr" << endl;
}

void stackOperations(Stack& stack) {
    int choice, data;
    do {
        cout << "\nStack Menu:\n";
        cout << "1. Push to stack\n";
        cout << "2. Pop from stack\n";
        cout << "3. Peek stack\n";
        cout << "4. Display stack\n";
        cout << "5. Return to main menu\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid choice, please enter a valid option (1-5): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                cout << "Enter data to push to stack: ";
                cin >> data;
                stack.push(data);
                break;
            case 2:
                stack.pop();
                break;
            case 3:
                cout << "Top of stack: " << stack.peek() << endl;
                break;
            case 4:
                cout << "Stack: ";
                stack.display();
                break;
            case 5:
                return;
        }
    } while (true);
}

void queueOperations(Queue& queue) {
    int choice, data;
    do {
        cout << "\nQueue Menu:\n";
        cout << "1. Enqueue to queue\n";
        cout << "2. Dequeue from queue\n";
        cout << "3. Peek queue\n";
        cout << "4. Display queue\n";
        cout << "5. Return to main menu\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid choice, please enter a valid option (1-5): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                cout << "Enter data to enqueue to queue: ";
                cin >> data;
                queue.enqueue(data);
                break;
            case 2:
                queue.dequeue();
                break;
            case 3:
                cout << "Front of queue: " << queue.peek() << endl;
                break;
            case 4:
                cout << "Queue: ";
                queue.display();
                break;
            case 5:
                return;
        }
    } while (true);
}

int main() {
    Stack stack;
    Queue queue;
    int choice;

    do {
        cout << "\nMain Menu:\n";
        cout << "1. Stack Operations\n";
        cout << "2. Queue Operations\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Invalid choice, please enter a valid option (1-3): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                stackOperations(stack);
                break;
            case 2:
                queueOperations(queue);
                break;
            case 3:
                cout << "Exiting program." << endl;
                return 0;
        }
    } while (true);

    return 0;
}
