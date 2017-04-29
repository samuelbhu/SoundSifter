#include "peakfinder.h"
#include "fftw3.h"
#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
	char dataFile[] = "1k.raw";
	int numpeaks;
	cin >> numpeaks;
	peakfinder peakClass(numpeaks);

	if(peakClass.getData(dataFile))
		cout << "SUCCESS!" << endl;
    peakClass.modulate();
    peakClass.takeFFT();
    peakClass.calcAvg();
    peakClass.findPeaks();
    peakClass.calcFilters();
    peakClass.printValues();
    
    
	return 0;
}