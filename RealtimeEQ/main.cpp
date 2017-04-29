//
//  main.cpp
//  ParaEq_PA_Filter
//
//  Created by Samuel Bhushan and Hailey Parkin on 10/24/15
//

#include <iostream>
#include "math.h"
#include "filter.h"
#include "PAfunc.h"

using namespace std;
int main(int argc, const char * argv[])
{
    EQfilter equalizer;
    char character;
    bool cont = true;
    float centerFreq = 10000, bandWidth = 2000, gaindB = 5;
    float gain = powf(10, gaindB/20);
    
    equalizer.getCoefficients(centerFreq, bandWidth, gain);
    PAinstance *PAClass = new PAinstance();
    PAClass->initializePA(&equalizer);
    PAClass->startPA();
    
    while(cont)
    {
        cout << "q to quit" << endl;
        cin >> character;
        if(character == 'q')
            cont = false;
        
    }
    PAClass->stopPA();
    PAClass->terminatePA();
    
    return 0;
}


