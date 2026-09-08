// Write a program that prompts the user to enter two integer values.
// Store these values in int variables named val1 and val2.
// Write your program to determine the smaller, larger, sum, difference, product, and ratio of these values and report them to the user.

#include <iostream>

using namespace std;

int main()
{
    int val1, val2;

    cout << "Enter two integer values: ";
    cin >> val1 >> val2;

    // Smaller and larger
    if (val1 < val2)
        cout << "Smaller: " << val1 << '\n';
    else
        cout << "Smaller: " << val2 << '\n';

    if (val1 > val2)
        cout << "Larger: " << val1 << '\n';
    else
        cout << "Larger: " << val2 << '\n';

    // Calculations
    cout << "Sum: " << val1 + val2 << '\n';
    cout << "Difference: " << val1 - val2 << '\n';
    cout << "Product: " << val1 * val2 << '\n';

    if (val2 != 0)
        cout << "Ratio: " << double(val1) / val2 << '\n';
    else
        cout << "Ratio: Cannot divide by zero\n";

    return 0;
}