//
//  portaudio.h
//  ParaEq_PA_Filter
//
//  Created by Samuel Bhushan on 10/24/15.
//  Copyright (c) 2015 Samuel Bhushan. All rights reserved.
//

#ifndef __ParaEq_PA_Filter__portaudio__
#define __ParaEq_PA_Filter__portaudio__

#include <stdio.h>
#include "portaudio.h"
#define SAMPLE_RATE (44100.)
#define INPUT_CHANNELS  (2)
#define OUTPUT_CHANNELS (2)
static int filterCallBack( const void *input,
                          void *output,
                          unsigned long frameCount,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData );

class PAinstance
{
public:
    bool initializePA(void* userData);
    bool startPA();
    bool stopPA();
    bool terminatePA();
    PAinstance();
private:
    static PAinstance* instance;
    PaStream *stream;
    PaStream * soundStream;
    PaError err;
    
    bool getInputDevice (PaStreamParameters& streamParams);
    bool getOutputDevice (PaStreamParameters& streamParams);
};



#endif /* defined(__ParaEq_PA_Filter__portaudio__) */
