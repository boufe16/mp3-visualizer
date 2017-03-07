#include "audio_data.h"

audio_data::audio_data(QByteArray &bytes)
{
    leftBuffer= new unsigned char[sizeof(bytes)/2];
    rightBuffer= new unsigned char[sizeof(bytes)/2];
    for(int i=0; i< sizeof(bytes)/2; i+=4)
    {
        leftBuffer[i] = ((unsigned char)bytes[i+1]<<8) | ((unsigned char)bytes[i]);
        rightBuffer[i]=((unsigned char)bytes[i+3]<<8) | ((unsigned char)bytes[i+2]);
               //little endian 16 =   ((unsigned char)bytes[21]<<8)|((unsigned char)bytes[20])
    }

}
