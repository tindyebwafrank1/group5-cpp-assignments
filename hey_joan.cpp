// A program that prompts for a first name and return a greeting.
#include<iostream>

using namespace std;

int main()
{
    cout << "Please enter your first name (followed by 'enter'):\n";
    string first_name; 
    cin >> first_name; 
    cout << "Hello, " << first_name << "!\n";

    return 0;
}