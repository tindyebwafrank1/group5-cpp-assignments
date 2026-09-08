// Rewrite the ascending.cpp program to process stings instead.
// So, if the user enters the values Electrical Eng., Computer Eng., Biomedical Eng.,
// the output should be Biomedical Eng., Computer Eng., Electrical Eng..

// Ascending.cpp program to process strings in ascending order
#include <iostream>
#include <string>
using namespace std;

int main() {

    string names[3];
    string temp;

    // Input the strings
    cout << "Enter 3 engineering fields:\n";

    for (int i = 0; i < 3; i++)
    {
        getline(cin, names[i]);
    }

    // Sort strings in ascending alphabetical order
    for (int i = 0; i < 2; i++)
    {
        for (int j = i + 1; j < 3; j++)
        {
            if (names[i] > names[j])
            {
                temp = names[i];
                names[i] = names[j];
                names[j] = temp;
            }
        }
    }

    // Display sorted strings
    cout << "\nStrings in ascending order:\n";

    for (int i = 0; i < 3; i++)
    {
        cout << names[i] << endl;
    }

    return 0;
}