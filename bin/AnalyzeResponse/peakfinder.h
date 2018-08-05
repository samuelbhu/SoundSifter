#ifndef _PEAKFINDER_H_
#define _PEAKFINDER_H_
#define SIZE 960000
#define FFTSIZE SIZE/2+1
#define TWENTYHZ 400

#define TWENTYKHZ 400000
#define TWOFIFTYHZ 5000
#define TENKHZ 200000
#define SAMPLERATE 48000.0
#define WINDOWSIZE 1000 // how far to look for avg in each dir
#
#define NUMBANDS 20


class peakfinder
{
private:
	double* rawData;
	double* smoothData;
	double* fftData;
    double* averagedBands;
    double* bandwidths;
	double avg;
    int filterCount;
	
public:
	peakfinder(int numFilters);
	~peakfinder();
	bool getData(char filename[]);
	void takeFFT();
	void smooth();
	double calcAvg();
	bool findPeaks();
    void printValues();
    void sort();
    void modulate();
    void calcFilters();

	int* peakidx;
	double* peakfreq; 
	double* peakgain;
	double* peakBW;

};

#endif