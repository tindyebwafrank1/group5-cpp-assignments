// The income tax rate for individuals in business depends on the income bracket in which the individual falls.
// Resident individuals enjoy a tax free annual income threshold of UGX. 2,820,000 per annum. The
// balance is taxed at 10%, 20% or 30% depending on the income bracket. Individuals who earn
// above UGX 120,000,000 pa pay an additional 10% on the income above 120m.
// +----------------------+------------------------------------------+------------------------------------------+
// | CHARGEABLE INCOME, CY| RESIDENTS                                | NON-RESIDENTS                             |
// | (UGX Annual)         | RATE OF TAX                              | RATE OF TAX                               |
// +----------------------+------------------------------------------+------------------------------------------+
// | 0 to 2,820,000       | Nil                                      | CY x 10%                                  |
// | 2,820,000 to         | (CY - 2,820,000) x 10%                   | CY x 10%                                  |
// | 4,020,000            |                                          |                                           |
// | 4,020,000 to         | (CY - 4,020,000) x 20% + 120,000         | (CY - 4,020,000) x 20% + 402,000          |
// | 4,920,000            |                                          |                                           |
// | 4,920,000 to         | (CY - 4,920,000) x 30% + 300,000         | (CY - 4,920,000) x 30% + 582,000          |
// | 120,000,000          |                                          |                                           |
// | Above 120,000,000    | [(CY - 4,920,000) x 30% + 300,000]       | [(CY - 4,920,000) x 30% + 582,000]        |
// |                      | + [(CY - 120,000,000) x 10%]             | + [(CY - 120,000,000) x 10%]              |
// +----------------------+------------------------------------------+------------------------------------------+

// You are to write a program to compute personal income tax. Your program should prompt
// the user to enter the residence status and taxable income and then compute the tax. Enter 0 for
// resident and 1 for non-resident.

// Output
// (0-Resident, 1-Non-resident)
// Enter the residence status: 0
// Enter the taxable income: 4,000,000
// Tax is 118000.

#include <iostream>
using namespace std;

int  main(){
    //Declarations
        int status;
            double income;
                double tax = 0;

                    //inputs
                        cout << "(0-Resident, 1-Non-resident)" << endl;
                            cout << "Enter the residence status: ";
                                cin >> status;

                                    cout << "Enter the taxable income: ";
                                        cin >> income;

                                            //Categories
                                                if(status == 0){
                                                        if(income <= 2820000){
                                                                    tax = 0;
                                                                            }
                                                                                    else if(income <= 4020000){
                                                                                                tax = (income - 2820000) * 0.1;
                                                                                                        }
                                                                                                                else if(income <= 4920000){
                                                                                                                            tax = (income - 4020000) * 0.2 + 120000;
                                                                                                                                    }
                                                                                                                                            else if(income <= 120000000){
                                                                                                                                                        tax = (income - 4920000) * 0.3 + 300000;
                                                                                                                                                                }
                                                                                                                                                                        else{
                                                                                                                                                                                    tax = (income - 4920000) * 0.3 + 300000 + (income - 120000000) * 0.1;
                                                                                                                                                                                            }
                                                                                                                                                                                                }
                                                                                                                                                                                                    else if( status == 1){
                                                                                                                                                                                                            if( income <= 2820000){
                                                                                                                                                                                                                        tax = income * 0.1;
                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                        else if(income <= 4020000){
                                                                                                                                                                                                                                                    tax = income * 0.1;
                                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                                                    else if(income <= 4920000){
                                                                                                                                                                                                                                                                                tax = (income - 4020000) * 0.2 + 402000;
                                                                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                                                                                else if(income <= 120000000){
                                                                                                                                                                                                                                                                                                            tax = (income - 4920000) * 0.3 + 582000;
                                                                                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                                                                                                            else{
                                                                                                                                                                                                                                                                                                                                        tax = (income - 4920000) * 0.3 + 582000 + (income - 120000000) * 0.1;
                                                                                                                                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                                                                                                                                    }

                                                                                                                                                                                                                                                                                                                                                        //Output
                                                                                                                                                                                                                                                                                                                                                            cout << "Tax is " << tax << "." << endl;

                                                                                                                                                                                                                                                                                                                                                                return 0;
                                                                                                                                                                                                                                                                                                                                                                }