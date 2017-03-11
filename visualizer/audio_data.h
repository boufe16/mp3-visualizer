#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <QByteArray>
#include <QDebug>
class audio_data
{
public:
    audio_data();
     audio_data(QByteArray &bytes);


     unsigned short * getAudio();
     unsigned long getNumSamples();
     unsigned long getSampleRate();
     unsigned int getNumChannels();

private:
    //short is 16 bits, whigh corresponds to the data length
    // left and right for stereo
     //only gonna need one channel, it'll be 2x the size if its stereo or whatever, Interleaved
     unsigned short *ch0;

     unsigned long numSamples;

     //not sure i'm going to leave it qu instead of c++ primitives
     QByteArray subchunk1ID;
     //all of these are little endian
     quint32 subchunk1Size, sampleRate, byteRate;
     quint16 audioFormat,numChannels, blockAlign,bitsPerSample;
     QByteArray subChunk2ID;
     quint32  subChunk2sz;
     QByteArray riff,chunkFormat;
     quint32 chunkSize;
};

#endif // AUDIO_DATA_H
