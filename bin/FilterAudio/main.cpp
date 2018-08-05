//
//  main.cpp
//  ParaEq_PA_Filter
//
//  Created by Samuel Bhushan on 10/24/15.
//  Copyright (c) 2015 Samuel Bhushan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "math.h"
#include "filter.h"
#include "PAfunc.h"

using namespace std;
int main(int argc, const char * argv[]) {

  bool cont = true;
  int numFilt = 0;
  float gain;
  char character;
  float centerFreq,bandWidth,gaindB;
  // Open FilterFile

  ifstream infile;

  infile.open("FilterData.txt");
  
  infile >> numFilt;
  std::cout << "Appling Filters" << std::endl;
  EQfilter* equalizer = new EQfilter[numFilt];
  for(int i = 0 ; i < numFilt; i++)
  {
    infile >> centerFreq;
    infile >> bandWidth;
    infile >> gaindB;
    gain = powf(10, gaindB/20);
    equalizer[i].getCoefficients(centerFreq, bandWidth, gain);
    equalizer[i].totalFilters = numFilt;
  }
  PAinstance *PAClass = new PAinstance();

    
  PAClass->initializePA(equalizer);
  cout << " initialized " << endl;
  PAClass->startPA();
  //while(1);
  while(cont)
  {
    cout << "q to quit" << endl;
    //cin.ignore();
    cin >> character;
    if(character == 'q')
      cont = false;
   }
  //while(1);
  PAClass->stopPA();
  PAClass->terminatePA();
 
  return 0;


  /*
  float centerFreq = 4000, bandWidth = 100, gaindB = 20;
  cout << "enter center freq:" << endl;
  cin >> centerFreq;
  cout << "enter bandwidth" << endl;
  cin >> bandWidth;
  //cout << "enter Gain" << endl;
  //cin >> gaindB;
  */
}


