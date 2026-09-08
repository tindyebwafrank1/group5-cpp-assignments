//Ascending.cpp program to process integers in ascending order
#include <iostream>
using namespace std;

int main()
{
    int a, b, c, temp;

    cout << "Enter three integer values: ";
    cin >> a >> b >> c;

    // Arrange the values in ascending order
    if (a > b)
    {
        temp = a;
        a = b;
        b = temp;
    }

    if (a > c)
    {
        temp = a;
        a = c;
        c = temp;
    }

    if (b > c)
    {
        temp = b;
        b = c;
        c = temp;
    }

    // Output the values separated by commas
    cout << a << ", " << b << ", " << c << endl;

    return 0;
}
