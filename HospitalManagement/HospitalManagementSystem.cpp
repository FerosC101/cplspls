#include <iostream>
#include <queue>
#include <string>
using namespace std;

struct Patient {
    int id;
    string name;
    int age;
    string medicalHistory;
    Patient* left;
    Patient* right;

    Patient(int i, string n, int a, string mh) 
        : id(i), name(n), age(a), medicalHistory(mh), left(nullptr), right(nullptr) {}
};

class PatientRecords {
private:
    Patient* root;

    Patient* insertPatient(Patient* node, int id, string name, int age, string medicalHistory) {
        if (node == nullptr)
            return new Patient(id, name, age, medicalHistory);
        if (id < node->id)
            node->left = insertPatient(node->left, id, name, age, medicalHistory);
        else if (id > node->id)
            node->right = insertPatient(node->right, id, name, age, medicalHistory);
        return node;
    }

    Patient* searchPatient(Patient* node, int id) {
        if (node == nullptr || node->id == id)
            return node;
        if (id < node->id)
            return searchPatient(node->left, id);
        return searchPatient(node->right, id);
    }

    Patient* findMin(Patient* node) {
        while (node && node->left != nullptr)
            node = node->left;
        return node;
    }

    Patient* deletePatient(Patient* node, int id) {
        if (node == nullptr)
            return node;
        if (id < node->id)
            node->left = deletePatient(node->left, id);
        else if (id > node->id)
            node->right = deletePatient(node->right, id);
        else {
            if (node->left == nullptr) {
                Patient* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Patient* temp = node->left;
                delete node;
                return temp;
            }
            Patient* temp = findMin(node->right);
            node->id = temp->id;
            node->name = temp->name;
            node->age = temp->age;
            node->medicalHistory = temp->medicalHistory;
            node->right = deletePatient(node->right, temp->id);
        }
        return node;
    }

    void inOrder(Patient* node) {
        if (node == nullptr)
            return;
        inOrder(node->left);
        cout << "ID: " << node->id << ", Name: " << node->name 
             << ", Age: " << node->age << ", Medical History: " << node->medicalHistory << endl;
        inOrder(node->right);
    }

public:
    PatientRecords() : root(nullptr) {}

    void addPatient(int id, string name, int age, string medicalHistory) {
        root = insertPatient(root, id, name, age, medicalHistory);
        cout << "Patient added successfully!" << endl;
    }

    void searchPatientByID(int id) {
        Patient* patient = searchPatient(root, id);
        if (patient) {
            cout << "Patient Found!" << endl;
            cout << "ID: " << patient->id << ", Name: " << patient->name 
                 << ", Age: " << patient->age << ", Medical History: " << patient->medicalHistory << endl;
        } else {
            cout << "Patient with ID " << id << " not found." << endl;
        }
    }

    void removePatient(int id) {
        root = deletePatient(root, id);
        cout << "Patient record removed successfully!" << endl;
    }

    void displayAllPatients() {
        if (root == nullptr) {
            cout << "No patient records available." << endl;
            return;
        }
        inOrder(root);
    }
};

class AppointmentQueue {
private:
    queue<int> appointmentQueue;

public:
    void scheduleAppointment(int patientID) {
        appointmentQueue.push(patientID);
        cout << "Appointment scheduled for Patient ID: " << patientID << endl;
    }

    void processNextAppointment() {
        if (!appointmentQueue.empty()) {
            int nextPatientID = appointmentQueue.front();
            appointmentQueue.pop();
            cout << "Processing appointment for Patient ID: " << nextPatientID << endl;
        } else {
            cout << "No pending appointments." << endl;
        }
    }

    void displayPendingAppointments() {
        if (appointmentQueue.empty()) {
            cout << "No pending appointments." << endl;
            return;
        }
        queue<int> temp = appointmentQueue;
        cout << "Pending Appointments (Patient IDs): ";
        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }
        cout << endl;
    }
};

void displayMenu() {
    cout << "\nHospital Management System Menu" << endl;
    cout << "1. Add a patient record" << endl;
    cout << "2. Remove a patient record" << endl;
    cout << "3. Search for a patient record by ID" << endl;
    cout << "4. Display all patient records" << endl;
    cout << "5. Schedule an appointment" << endl;
    cout << "6. Process the next appointment" << endl;
    cout << "7. Display pending appointments" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    PatientRecords records;
    AppointmentQueue appointments;
    int choice, id, age;
    string name, medicalHistory;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter patient ID: ";
                cin >> id;
                cin.ignore();  
                cout << "Enter patient name: ";
                getline(cin, name);
                cout << "Enter patient age: ";
                cin >> age;
                cin.ignore();  
                cout << "Enter patient medical history: ";
                getline(cin, medicalHistory);
                records.addPatient(id, name, age, medicalHistory);
                break;
            case 2:
                cout << "Enter patient ID to remove: ";
                cin >> id;
                records.removePatient(id);
                break;
            case 3:
                cout << "Enter patient ID to search: ";
                cin >> id;
                records.searchPatientByID(id);
                break;
            case 4:
                records.displayAllPatients();
                break;
            case 5:
                cout << "Enter patient ID to schedule an appointment: ";
                cin >> id;
                appointments.scheduleAppointment(id);
                break;
            case 6:
                appointments.processNextAppointment();
                break;
            case 7:
                appointments.displayPendingAppointments();
                break;
            case 0:
                cout << "Exiting the system." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
