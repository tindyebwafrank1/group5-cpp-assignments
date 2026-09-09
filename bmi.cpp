// Write a program that prompts the user to enter a weight in pounds and height in inches and
// then displays the BMI. Note that one pound is 0.45359237 kilograms and one inch is
// 0.0254 meters. Listing 4.6 gives the program.

// Conditions
// Below 18.5 Underweight
// 18.5–24.9 Normal
// 25.0–29.9 Overweight
// Above 30.0 Obese

// Enter weight in pounds:
// Enter Weight in pounds: 146
// Enter height in inches: 70
// BMI is 20.95
// Normal

#include<iostream>
#include <iomanip>
using namespace std;

int main(){
    //Declarations
    double weight, height, bmi;
    double mass_kg, height_m;

    //Input
    cout << "Enter your weight in pounds: ";
    cin >> weight;

    cout << "Enter your height in inches: ";
    cin >> height;

    //Calculations
    mass_kg = weight * 0.45359237;
    height_m = height * 0.0254;

    bmi = mass_kg/((height_m)*(height_m));

    //Output
    cout << setprecision(4) << "\n" << "BMI is: " << bmi << endl;   //cout << setprecision(2);

    //Categories
    if(bmi < 18.5){
        cout << "Underweight";
    }
    else if(bmi < 25.0 ){
        cout << "Normal";
    }
    else if(bmi < 30.0){
        cout <<"Overweight";
    }
    else{
        cout << "Obese";
    }
    
    return 0;
}
