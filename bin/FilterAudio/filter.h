//
//  filter.h
//  ParaEq_PA_Filter
//
//  Created by Samuel Bhushan on 10/24/15.
//  Copyright (c) 2015 Samuel Bhushan. All rights reserved.
//

#ifndef __ParaEq_PA_Filter__filter__
#define __ParaEq_PA_Filter__filter__

#include <stdio.h>
#include <math.h>

class EQfilter
{
 public:
  float processData(float input);
  void getCoefficients(float CF,float BW, float G); //G is NOT dB
  EQfilter();
  int totalFilters;
 private:
  float a0;
  float a1;
  float a2;
  float b0;
  float b1;
  float b2;
  float* buffer;
  float* bufferin;
    
};

#endif /* defined(__ParaEq_PA_Filter__filter__) */
