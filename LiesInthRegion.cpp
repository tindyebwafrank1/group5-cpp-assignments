#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// Structure to hold individual 2D coordinate points
struct Point {
    double x;
    double y;
};

// Function that evaluates whether a point (x, y) falls within the region
bool isPointInside(double x, double y) {
    // 1. Check absolute horizontal limits [-5.0 to 4.5]
    if (x < -5.0 || x > 4.5) {
        return false;
    }

    // 2. Compute lower boundary y-value based on current x-coordinate
    double y_lower;
    if (x <= -3.0) {
        y_lower = 2.0 * x + 5.0; // Line: y = 2x + 5
    } else {
        y_lower = -1.0 - sin(x); // Sine wave: y = -1 - sin(x)
    }

    // 3. Compute upper boundary y-value based on current x-coordinate
    double y_upper;
    if (x <= -2.0) {
        y_upper = pow(x, 2) + 2.0; // Parabola: y = x^2 + 2
    } else if (x <= 3.0) {
        y_upper = 4.0 + sin(x); // Sine wave: y = 4 + sin(x)
    } else if (x <= 4.2) {
        y_upper = -2.0 * x + 10.0; // Line: y = -2x + 10
    } else {
        y_upper = sqrt(9.0 - pow(x - 1.0, 2)); // Circle arc: y = sqrt(9 - (x - 1)^2)
    }

    // 4. Verify if y-coordinate lies between lower and upper bounds
    return (y >= y_lower && y <= y_upper);
}

int main() {
    double x, y;
    bool validRange = false;

    // Looping until the user enters an x-coordinate within the valid horizontal bounds
    while (!validRange) {
        cout << "Enter x coordinate:" << endl;
        if (!(cin >> x)) {
            cout << "Invalid input format." << endl;
            return 1;
        }

        cout << "Enter y coordinate:" << endl;
        if (!(cin >> y)) {
            cout << "Invalid input format." << endl;
            return 1;
        }

        // Validate if x falls within the region's total horizontal boundary [-5, 4.5]
        if (x < -5.0 || x > 4.5) {
            cout << "Point is completely out of the region's range. Please try again." << endl;
            cout << endl;
        } else {
            validRange = true; // Valid range provided, exit loop
        }
    }

    // Determine and display final evaluation for valid input
    if (isPointInside(x, y)) {
        cout << "The point (" << x << ", " << y << ") lies inside the region." << endl;
    } else {
        cout << "The point (" << x << ", " << y << ") lies outside the region." << endl;
    }

    // Batch verification section
    vector<Point> batchPoints = {
        {1.0, 1.0},    // p1 (Inside)
        {-4.0, -3.0},  // p2 (Outside)
        {0.0, 3.0},    // Inside
        {3.5, 2.0},    // Inside
        {-4.5, 1.0},   // Outside
        {2.0, 5.0}     // Outside
    };

    cout << endl;
    cout << "Checking sample batch of coordinates:" << endl;

    for (const auto& p : batchPoints) {
        if (isPointInside(p.x, p.y)) {
            cout << "Point (" << p.x << ", " << p.y << ") is INSIDE" << endl;
        } else {
            cout << "Point (" << p.x << ", " << p.y << ") is OUTSIDE" << endl;
        }
    }

    return 0;
}