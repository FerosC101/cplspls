#include <iostream>
#include <ctime>

using namespace std;

int main() 
{
    int num;
    int guess;
    int tries;

    srand(time(NULL));
    num = (rand() % 100) + 1;

    cout << "*********** NUMBER GUESSING GAME ***********\n";

    do{
        cout << "Enter a guess between (1-100): ";
        cin >> guess;
        tries++;

        if(guess > num) {
            cout << " bes taas! \n";
        } else if(guess < num) {
            cout << " bes baba! \n";
        } else {
            cout << "CORRECT! # of tries: " << tries << '\n';
        }

    }while(guess != num);

    cout << "*********** NUMBER GUESSING GAME ***********\n";

    return 0;
}

