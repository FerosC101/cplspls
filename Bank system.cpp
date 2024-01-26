#include <iostream>
#include <iomanip>

using namespace std;

void MyBalance(double balance);
double deposit();
double withdraw(double balance);

int main()
{
    double balance = 0;
    int choice = 0;

    do{
        cout << "*******************\n";
        cout << "Enter your choice: \n";
        cout << "*******************\n";
        cout << "1. Show Balance\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Moeny\n";
        cout << "4. Exit \n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice) {
            case 1: MyBalance(balance);
                break;
            case 2: balance = balance + deposit();
                    MyBalance(balance);
                    break; 
            case 3: balance = balance - withdraw(balance);
                    MyBalance(balance);
                    break;
            case 4: cout << "Thank you for using!";
                    break;
            default: cout << "Invalid choice!";
                    break;
        }
    }while(choice != 4);

    return 0;
}

void MyBalance(double balance) {
    cout << "Your current balance is: $" << setprecision(2) << fixed << balance << '\n';
}
double deposit() {
    double amount = 0;

    cout << "Enter amout to deposit: ";
    cin >> amount;

    if(amount > 0) {
        return amount;
    } else {
        cout << "Invalid amount\n";
        return 0;
       
    }

}
double withdraw(double balance) {

    double amount = 0;

    cout << "Enter amount to withdraw: ";
    cin >> amount;

    if(amount > balance) {
        cout << "Insufficient funds\n";
        return 0;
    } else if(amount < 0) {
        cout << "Invalid amount\n";
        return 0;
    } else {
        return amount;
    }

}