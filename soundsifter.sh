#!/bin/bash                                                                  
aplay white.wav -d1 > /dev/null &
arecord -f S24_3LE -r 48000 -c 2 test.wav -d 1 -t WAV -D hw:1 > /dev/null
MAXVOL=$(sox test.wav -n stats 2>&1 | grep Max | awk 'NR==1{print $3}')
LOUDENOUGH=$(./checkVol.py $MAXVOL)

while (($LOUDENOUGH == 0))
do
    echo 'too quiet'
    # Get The Second Recording again
    aplay white.wav -d 1 > /dev/null &  
    arecord -f S24_3LE -r 48000 -c 2 test.wav -d 1 -t WAV -D hw:1 > /dev/null 
    MAXVOL=$(sox test.wav -n stats 2>&1 | grep Max | awk 'NR==1{print $3}')
    echo $MAXVOL
    LOUDENOUGH=$(./checkVol.py $MAXVOL)
done

echo 'loud enough'
aplay beep.wav
aplay SweepingSine20-20000kHz.wav &                                         
arecord -f S24_3LE -r 48000 -c 2 recorded_sweep.raw -d 20 -t RAW -D hw:1 


