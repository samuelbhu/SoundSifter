#include "peakfinder.h"
#include "fftw3.h"
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <cstring>

double centerFArr[20] = {
    23.7700445487404,
    33.5760803624512,
    47.4274741132331,
    66.9930878315656,
    94.6302517922961,
    133.668783513723,
    188.812175257185,
    266.704286432665,
    376.729817897960,
    532.145011959762,
    751.674808576889,
    1061.76888846198,
    1499.78841866491,
    2118.50745035458,
    2992.47131218887,
    4226.97807967330,
    5970.76523783592,
    8433.93006857165,
    11913.2428705802,
    16827.9028329039};
struct int24 
{
	int data : 24;
};

peakfinder::peakfinder(int numFilters)
{
    filterCount = numFilters;
	rawData = new double[SIZE];
	fftData = new double[FFTSIZE];
	smoothData = new double[FFTSIZE];
	peakidx = new int[numFilters];
	peakfreq = new double[numFilters];
	peakgain = new double[numFilters];
	peakBW = new double[numFilters];
    averagedBands = new double[NUMBANDS];
    bandwidths = new double[NUMBANDS];
    for (int i=0; i < numFilters; i++) {
        peakgain[i] = 0;
    }
	avg = 0;
}
peakfinder::~peakfinder()
{
	delete[] rawData ;
	delete[] fftData ;
	delete[] smoothData;
	delete[] peakidx ;
	delete[] peakfreq;
	delete[] peakgain;
	delete[] peakBW;
    delete[] averagedBands;
    delete[] bandwidths;

}
bool peakfinder::getData(char filename[])
{
	std::fstream file;
	int24 threeBytes;
	int* tempInt = new int;
	file.open(filename,std::fstream::in | std::fstream::binary);
	if(file.fail())
	{
		printf("file open error");
		return 0;
	}
	printf("file open success");

  	for(int i=0; i<SIZE && !file.eof();i++)
    {
        file.read((char*)tempInt,3);
        memcpy(&threeBytes,tempInt,3);
        rawData[i] = (double)threeBytes.data;
        file.seekg(3,file.cur);
    }
	file.close();
	return 1;
}
void peakfinder::modulate()
{
    double t;
    double wt,nt,l;
    l = (20)/(log(20000*2*M_PI/(20*2*M_PI)));
    for (int i = 0; i < SIZE; i++) {
        t = (i/48000.0);
        wt = 20*2*M_PI * exp(t/l);
        nt = sqrt(wt/(2*20*M_PI));
        rawData[i] = nt*rawData[i];
    }
}
void peakfinder::takeFFT()
{
	fftw_complex *out;
	fftw_plan fftplan;
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*SIZE);
	fftplan = fftw_plan_dft_r2c_1d(SIZE,rawData,out,FFTW_ESTIMATE);
	fftw_execute(fftplan);
	for (int i = 0; i < FFTSIZE; ++i)
	{
        fftData[i] = sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1]);
	}
	fftw_free(out);

}

double peakfinder::calcAvg()
{
	for (int i = TWOFIFTYHZ ; i < TENKHZ; ++i)
	{
		avg += fftData[i] / (TENKHZ - TWOFIFTYHZ);
	}
    std::cout << "WHOLE Average: " << avg << std::endl;
    int startbin;
    int endbin;
    double frequency = 20;
    double nextfreq;
    double average;
    
    for (int i = 0; i < 20; i++) {
        nextfreq = frequency*pow(10, 3.0/20.0);
        startbin = frequency*SIZE/SAMPLERATE;
        endbin = nextfreq*SIZE/SAMPLERATE;
        average = 0;
        for (int j = startbin;j < endbin; j++) {
            average += fftData[j]/(endbin-startbin);
        }
        averagedBands[i] = average;
        bandwidths[i] = nextfreq - frequency;
        frequency = nextfreq;
    }
    for (int i = 0; i < NUMBANDS; i++) {
        std::cout << "avg of: " << i << "is: " << averagedBands[i] << std::endl;
    }
    return avg;

}

// we take the given boosts at 20 locations and choose the top numfilters of them
bool peakfinder::findPeaks()
{
    int lowestidx;
    double lowestdeviation;
    double curdeviation;
    
    for (int i = 0; i < NUMBANDS - filterCount; i++)
    {
        lowestidx = 0;
        lowestdeviation = fabs(averagedBands[0] - avg);
        for (int j = 0; j < NUMBANDS; j++)
        {
            curdeviation = fabs(averagedBands[j]-avg);
            if(curdeviation < lowestdeviation && averagedBands[j] != 0)
            {
                lowestdeviation = curdeviation;
                lowestidx = j;
            }
        }
        averagedBands[lowestidx] = 0;
    }
    for (int i = 0; i < NUMBANDS; i++) {
         std::cout << "avg of: " << i << "is: " << averagedBands[i] << std::endl;
    }
    return true;
}

void peakfinder::calcFilters()
{
    int curfilter = 0;
    int deviation;
    for (int i = 0; i < NUMBANDS; i++) {
        if(averagedBands[i]!=0)
        {
            //Get the areas to change
            deviation =  10*log10(avg) - 10*log10(averagedBands[i]);
            peakgain[curfilter] = deviation;  //in dB
            peakfreq[curfilter] = centerFArr[i];
            peakBW[curfilter] = bandwidths[i];
            std::cout << "Bandwidths: " << bandwidths[i] << std::endl;
            curfilter++;
        }
    }
}
void peakfinder::printValues()
{
    std:: ofstream outFile;

    outFile.open("FilterData.txt");

    outFile << filterCount << std::endl;

    for (int i = 0; i < filterCount; ++i)
    {
      outFile << peakfreq[i]  << "\t";
      outFile << peakBW[i]    << "\t";
        outFile << peakgain[i] << std:: endl; 
    }
    outFile.close();
    









    /*std::ofstream fs;
    fs.open("modulated.txt");
    fs << "index \t" << "modulatedSweep" << std::endl;
    for (int i=0; i<SIZE; i++) {
        fs << i << "\t" << rawData[i] << std::endl;
    }
    fs.close();
    fs.open("fft.txt");
    fs << "FrequencyBin \t" << "FFT" << std::endl;
    for (int i=0; i<FFTSIZE; i++) {
        fs << i*SAMPLERATE/SIZE << "\t" << fftData[i] << std::endl;
    }*/
}
//////////////UNUSED STUFF BELOW ///////////////////
void peakfinder::smooth()
{
    int avgwidth;
    int N;
    double average = 0;
    for (int i = TWENTYHZ; i < TWENTYKHZ; ++i)
    {
        // we want to look avgwidth on each direction
        avgwidth = std::min(i-TWENTYHZ	 , TWENTYKHZ-1 - i);
        if(avgwidth > WINDOWSIZE)
            avgwidth = WINDOWSIZE;
        
        N = 2*avgwidth+1;
        average = 0;
        for(int j = i-avgwidth; j <= i+avgwidth; j++)
        {
            average += fftData[j]/N;
        }
        smoothData[i] = average;
    }
}
void peakfinder::sort()
{
    // arr = peakgain[]
    int min = 0;
    double temp;
    
    for (int i = 0; i < sizeof(peakgain); i++) {
        min = i;
        for (int j = i; j < sizeof(peakgain); j++) {
            if(peakgain[j] < peakgain[min] )
                min = j;
            
        }
        temp = peakgain[min];
        peakgain[min] = peakgain[i];
        peakgain[i] = temp;
        
    }
}
