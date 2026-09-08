//A unit converter from miles to kilometers
#include <iostream>

using namespace std;

int main()
{
    double miles, kilometers;

    cout << "Enter the number of miles: ";
    cin >> miles;

    kilometers = miles * 1.609;

    cout << miles << " miles is " << kilometers << " kilometers." << endl;

    return 0;
}