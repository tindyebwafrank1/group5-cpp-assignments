// (Population projection) UBoS projects population based on the
// following assumptions:
// One birth every 7 seconds
// One death every 13 seconds
// One new immigrant every 45 seconds
// Write a program to display the population for each of the next five years. Assume the current population is 312032486 and one year has 365 days.

#include <iostream>
using namespace std;

int main(){
    //Declarations
    int population = 312032486;
    int births;
    int deaths;
    int immigrants;
    int secondsPerYear;

    //Calculations
    secondsPerYear = 365*24*60*60;

    births = secondsPerYear/7;

    deaths = secondsPerYear/13;

    immigrants = secondsPerYear/45;

    //looping
    for(int i = 0; i < 5; i++){
        
        population = population + births + immigrants - deaths;

        cout << "Year " << i + 1 << " population is: " << population << endl;
    }

    return 0;
}