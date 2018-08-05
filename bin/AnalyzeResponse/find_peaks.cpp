#include "peakfinder.h"
#include "fftw3.h"
#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
	char dataFile[] = "recorded_sweep.raw";
	int numpeaks;
	//cin >> numpeaks;
	numpeaks = 6;
	peakfinder peakClass(numpeaks);

	if(peakClass.getData(dataFile))
		cout << "SUCCESS!" << endl;
    peakClass.modulate();
    cout << "Modulate Done" << endl;
    peakClass.takeFFT();
    cout << "FFT Taken " << endl;
    peakClass.calcAvg();
    cout << "Average Calc'd" << endl;
    peakClass.findPeaks();
    cout << "Peaks Found " << endl;
    peakClass.calcFilters();
    peakClass.printValues();
    
    
	return 0;
}
