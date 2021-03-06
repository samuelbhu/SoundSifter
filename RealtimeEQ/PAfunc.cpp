//
//  portaudio.cpp
//  ParaEq_PA_Filter
//
//  Created by Samuel Bhushan on 10/24/15.
//  Copyright (c) 2015 Samuel Bhushan. All rights reserved.
//

#include "PAfunc.h"
#include "portaudio.h"
#include "filter.h"
#include <iostream>

//Port Audio Callback

static int filterCallBack( const void *input,
                             void *output,
                             unsigned long frameCount,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData )
{
    //Convert void* to
    
    const float* insample = (const float*)input;
    float* outsample = (float*)output;
    EQfilter* filterClass = (EQfilter*)userData;
    
    for (int i = 0;i < frameCount; i++) {
        *outsample++ = filterClass->processData(*insample++);
        *outsample++ = filterClass->processData(*insample++);
    }
    return 0;
}

PAinstance::PAinstance(): stream(NULL)
{
    //asdf
}
bool PAinstance::initializePA(void *userData)
{
    PaStreamParameters inputDevice,outputDevice;
    err = Pa_Initialize();
    if(err != paNoError)
    {
        printf("Init Error: %s \n",Pa_GetErrorText(err));
        return false;
    }
    getInputDevice(inputDevice);
    getOutputDevice(outputDevice);
    
    err = Pa_OpenStream(&stream, &inputDevice, &outputDevice, SAMPLE_RATE, 256, paClipOff,filterCallBack,userData);
    if(err != paNoError)
    {
        printf("Init Error: %s \n",Pa_GetErrorText(err));
        return false;
    }
    return true;
}


bool PAinstance::startPA()
{
    err = Pa_StartStream(stream);
    if(err != paNoError)
    {
        printf("Start Error: %s \n",Pa_GetErrorText(err));
        return false;
    }
    return true;
}
bool PAinstance::stopPA()
{
    err = Pa_StopStream(&stream);
    if(err != paNoError)
    {
        printf("Stop Error: %s \n",Pa_GetErrorText(err));
        return false;
    }
    return true;
}
bool PAinstance::terminatePA()
{
    err = Pa_Terminate();
    if(err != paNoError)
    {
        printf("Terminate Error: %s \n",Pa_GetErrorText(err));
        return false;
    }
    return true;
}

bool PAinstance::getInputDevice(PaStreamParameters &streamParams)
{
    PaDeviceIndex inputDevice = Pa_GetDefaultInputDevice();
    if (inputDevice == paNoDevice)
    {
       printf("Error: no default input device.\n");
        return false;
    }
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(inputDevice);
    
    streamParams.device = inputDevice;
    streamParams.channelCount = INPUT_CHANNELS;
    streamParams.sampleFormat = paFloat32;
    streamParams.suggestedLatency = deviceInfo->defaultLowOutputLatency;
    streamParams.hostApiSpecificStreamInfo = NULL;
    return true;
}
bool PAinstance::getOutputDevice(PaStreamParameters &streamParams)
{
    PaDeviceIndex outputDevice = Pa_GetDefaultOutputDevice();
    if (outputDevice == paNoDevice)
    {
        printf("Error: no default output device.\n");
        return false;
    }
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(outputDevice);
    
    streamParams.device = outputDevice;
    streamParams.channelCount = OUTPUT_CHANNELS;
    streamParams.sampleFormat = paFloat32;
    streamParams.suggestedLatency = deviceInfo->defaultLowOutputLatency;
    streamParams.hostApiSpecificStreamInfo = NULL;
    return true;
}












