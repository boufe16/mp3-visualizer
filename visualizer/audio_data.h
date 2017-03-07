#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <QByteArray>
#include <QDebug>
class audio_data
{
public:
    audio_data();
     audio_data(QByteArray bytes);
     void set_data(QByteArray bytes);
     unsigned short * getStereoLeft();
     unsigned short * getStereoRight();
     unsigned short * getMono();
     unsigned long getNumSamples();
     unsigned long getSampleRate();


private:
    //short is 16 bits, whigh corresponds to the data length
    // left and right for stereo
     unsigned short *ch0;
     unsigned short *ch1;
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
