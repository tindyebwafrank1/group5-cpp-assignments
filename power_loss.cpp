// Power loss is formally defined as the product of resistance and the square of the current.
// Resistance is defined as resistivity * (L / A), where L is the length and A is the cross-sectional area.

// Write a program that prompts the user to enter L, A, resistivity, and current,
// and returns the power loss of a transmission line.

#include <iostream>

using namespace std;

int main()
{
    double L, A, resistivity, current;
    double resistance, powerLoss;

    cout << "Enter the length of the transmission line (L): ";
    cin >> L;

    cout << "Enter the cross-sectional area (A): ";
    cin >> A;

    cout << "Enter the resistivity: ";
    cin >> resistivity;

    cout << "Enter the current: ";
    cin >> current;

    // Calculate resistance
    resistance = resistivity * (L / A);

    // Calculate power loss
    powerLoss = resistance * current * current;

    cout << "Resistance = " << resistance << '\n';
    cout << "Power loss = " << powerLoss << " W\n";

    return 0;
}