#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    cout << "***********BMI CALCULATOR***********" << endl;

    string name;

    cout << "Hello Welcome to BMI CALCULATOR, what is your name?: ";
    getline(cin, name);
    cout << "Hello " << name << "!" << endl;

    int kg;
    float met;

    cout << "Enter your weight in kilograms: ";
    cin >> kg;
    cout << "Enter your height in meters: ";
    cin >> met;
    cout << "\n";

    float BMI = kg/(pow(met, 2));

    if (BMI < 18.5) {        

        cout << "Hello " << name << "," << " your BMI is" << setprecision(2) << BMI << " You are underweight!";

    } else if (BMI >= 18.5 && BMI < 25.0) {
        
        cout << "Hello " << name << "," << " your BMI is" << setprecision(2) << BMI << " You are healthy!";

    } else if (BMI >= 25.0 and BMI < 30.0) {
        
        cout << "Hello " << name << "," << " your BMI is" << setprecision(2) << BMI << " You are overweight!";

    } else if (BMI >= 30.0) {
        
        cout << "Hello " << name << "," << " your BMI is" << setprecision(2) << BMI << " You are obese!";

    } else {
        cout << "Invalid Input";
    }

    return 0;

}