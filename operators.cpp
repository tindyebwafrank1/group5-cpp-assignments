// Write a program that takes an operation followed by two operands and outputs the result. For example:
// + 100 3.14
// ∗ 4 5

// Read the operation into a string called operation and use an if-statement to figure out which
// operation the user wants, for example, if (operation=="+"). Read the operands into variables
// of type double. Implement this for operations called +, −, ∗, /, plus, minus, mul, and div with
// their obvious meanings.

#include <iostream>
#include <string>

using namespace std;

int main()
{
    string operation;
    double val1, val2;

    cout << "Enter an operation followed by two operands: ";
    cin >> operation >> val1 >> val2;

    if (operation == "+" || operation == "plus")
        cout << val1 + val2 << '\n';

    else if (operation == "-" || operation == "minus")
        cout << val1 - val2 << '\n';

    else if (operation == "*" || operation == "mul")
        cout << val1 * val2 << '\n';

    else if (operation == "/" || operation == "div")
    {
        if (val2 != 0)
            cout << val1 / val2 << '\n';
        else
            cout << "Error: Cannot divide by zero\n";
    }

    else
        cout << "Error: Unknown operation\n";

    return 0;
}