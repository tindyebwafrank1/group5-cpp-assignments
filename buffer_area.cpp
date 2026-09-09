/*## Buffer Analysis

A buffer region is an area created around a geographic feature at a specified distance.

For example, a 1 km buffer around a water point represents all locations that are within 1 km of that water source.

Buffers can be created around points, lines, or polygons and are commonly used in GIS for proximity and spatial analysis.

### Task

Write a program that:

- Takes a specific point coordinate as the center location.
- Creates buffers of different sizes around that point.
- Takes a given set of points.
- Determines which points fall within each buffer.
- Returns the points contained in each buffer region*/

    #include <iostream>
    #include <vector>
    #include <cmath>
    using namespace std;

    int main(){
        //Declarations
        double centerX, centerY;
        vector<pair<double, double>> points;
        vector<double> bufferSizes;

        //inputs
        cout << "Enter the center point coordinates (x y): ";
        cin >> centerX >> centerY;

        // Example buffer sizes (can be modified)
        bufferSizes.push_back(1.0);
        bufferSizes.push_back(2.0);
        bufferSizes.push_back(5.0);

        // Example points (can be modified)
        points.push_back({0.0, 0.0});
        points.push_back({3.0, 4.0});
        points.push_back({6.0, 8.0});

        // Create buffers and determine which points fall within each
        for (double size : bufferSizes) {
            cout << "Buffer of size " << size << ":" << endl;
            for (const auto& point : points) {
                double distance = sqrt(pow(point.first - centerX, 2) + pow(point.second - centerY, 2));
                if (distance <= size) {
                    cout << "  Point (" << point.first << ", " << point.second << ")" << endl;
                }
            }
        }

        return 0;
    }