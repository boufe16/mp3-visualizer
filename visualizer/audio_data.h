#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <QByteArray>
class audio_data
{
public:
    audio_data();
     audio_data(QByteArray &bytes);
     unsigned char *leftBuffer;
     unsigned char *rightBuffer;
private:
    //short is 16 bits, whigh corresponds to the data length
    // left and right for stereo



};

#endif // AUDIO_DATA_H
