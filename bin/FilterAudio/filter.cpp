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
  float out;
  out = (b0*input + b1*bufferin[0] + b2*bufferin[1] -
	 a1*buffer[0] - a2*buffer[1])/(a0);
  bufferin[1] = bufferin[0];
  bufferin[0] = input;
  buffer[1] = buffer[0];
  buffer[0] = out;
  return out; 




  /*  
  buffer[0] = input - a1*buffer[1] - a2*buffer[2];
  float out = b0*buffer[0] + b1*buffer[1] + b2*buffer[2];
  buffer[2] = buffer[1];
  buffer[1] = buffer[0];
  */
  //return out;
}
EQfilter::EQfilter()
{
  a0 = 1;
  a1 = 1;
  a2 = 1;
  b0 = 1;
  b1 = 1;
  b2 = 1;
  
  buffer = new float[3];
  bufferin = new float[3];
  for (int i = 0; i < 3; i++) {
    buffer[i] = 0;
    bufferin[i] = 0;
  }
}
void EQfilter::getCoefficients(float CF, float BW, float G)
{
  //Gain comes in as a ratio
  float Q = CF/BW;

  float f = CF;
  float g = G;
  float q = Q;
  float A, omega, cs, sn, alpha;

  A = pow(10,g/40.0f);
  omega = (2* M_PI * f) / SAMPLE_RATE;
  sn = sin(omega);
  cs = cos(omega);
  alpha = sn/(2.0 * q);

  b0 = 1 + (alpha * A);
  b1 = -2*cs;
  b2 = 1 - (alpha * A);
  a0 = 1 + (alpha / (float)A);
  a1 = -2 * cs;
  a2 = 1 - (alpha / (float)A);

  // CF, Q and G



  /*
  Q = Q/G;
  // G = 0.5012;
  //Q = 4;
  //CF = 2000;
  float B = CF/Q;
  float b,a;
  b = -cos(2*M_PI*CF/SAMPLE_RATE);
  a = ( 1 - tan(M_PI * B / SAMPLE_RATE ))/(1 + tan(M_PI * B / SAMPLE_RATE ));
  b0 = 0.5 * (1 + a + G - G * a);
  b1 = b + b * a;
  b2 = 0.5 * (1 + a - G + G * a );
  a1 = b; ///?
  a2 = a;
  */
}






