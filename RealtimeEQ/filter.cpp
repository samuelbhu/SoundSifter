//
//  filter.cpp
//  ParaEq_PA_Filter
//
//  Created by Samuel Bhushan on 10/24/15.
//  Copyright (c) 2015 Samuel Bhushan. All rights reserved.
//

#include "filter.h"
#include <math.h>
#include "PAfunc.h"


float EQfilter::processData(float input)
{
    
    buffer[0] = input - a1*buffer[1] - a2*buffer[2];
    float out = b0*buffer[0] + b1*buffer[1] + b2*buffer[2];
    buffer[2] = buffer[1];
    buffer[1] = buffer[0];
    
    return out;
}
EQfilter::EQfilter()
{
    a1 = 1;
    a2 = 1;
    b0 = 1;
    b1 = 1;
    b2 = 1;
    buffer = new float[3];
    for (int i = 0; i < sizeof(buffer); i++) {
        buffer[i] = 0;
    }
}
void EQfilter::getCoefficients(float CF, float BW, float G)
{
    //Gain comes in as a ratio
    float Q = CF/BW;
    Q = Q/G;
    float B = CF/Q;
    float b,a;
    
    b = -cos(2*M_PI*CF/SAMPLE_RATE);
    a = ( 1 - tan(M_PI * B / SAMPLE_RATE ))/(1 + tan(M_PI * B / SAMPLE_RATE ));
    b0 = 0.5 * (1 + a + G - G * a);
    b1 = b + b * a;
    b2 = 0.5 * (1 + a - G + G * a );
    a1 = b;
    a2 = a;
    
}






