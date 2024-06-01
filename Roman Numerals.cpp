#include <iostream>
#include <unordered_map>

using namespace std;

int RomNum(string s) {

    int result = 0;

    unordered_map <char, int> equivalent = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}
    };

    for (int x = 0; x < s.length(); x++) {
        if (equivalent[s[x+1]]) {
            result -= equivalent[s[x]];
        } else {
            result += equivalent[s[x]];
        }
    }
    return result;
}

int main() {

    string rmanNum;

    cout << "Enter Roman Numeral: ";
    cin >> rmanNum;

    int conversion = RomNum(rmanNum);

    cout << rmanNum << " = " << conversion;

    return 0;
}