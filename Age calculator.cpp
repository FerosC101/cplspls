#include <iostream>

using namespace std;

int calculateAge(int birthYear, int birthMonth, int birthDay, int currentYear, int currentMonth, int currentDay) {

    int age;

    age = currentYear - birthYear;

    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }
    return age;
}

int main() {
    
    int birthYear, birthMonth, birthDay;
    int currentYear, currentMonth, currentDay;

    while(true) {
    
    do {
    cout << "Enter your birth year: ";
    cin >> birthYear;
    cout << "Enter your birth month: ";
    cin >> birthMonth;
    cout << "ENter your birth day: ";
    cin >> birthYear;
    } while(birthMonth > 12 || birthDay > 31);

    do {
    cout << "Enter current year: ";
    cin >> currentYear;
    cout << "Enter current month: ";
    cin >> currentMonth;
    cout << "Enter current day: ";
    cin >> currentDay;
    } while(birthMonth > 12 || birthDay > 31);

        if(birthYear < currentYear) {
            break;
        }
    }

    int age = calculateAge(birthYear, birthMonth, birthDay, currentYear, currentMonth, currentDay);

    cout << "You are now " << age << "year/s old";

    return 0;


}