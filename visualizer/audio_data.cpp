#include "audio_data.h"
audio_data::audio_data()
{

}
audio_data::audio_data(QByteArray bytes)
{ //need to clear data on new file

    //subchunk 1

        riff.append(bytes,4);

//        qDebug()<<(cstemp[0]<<24);

        chunkSize = ((unsigned char)bytes[7]<<24)|((unsigned char)bytes[6]<<16)|((unsigned char)bytes[5]<<8)|((unsigned char)bytes[4]);//make sure unsigned char

        chunkFormat.append(bytes, 12);
        chunkFormat.remove(0,8);

    qDebug()<<riff;
    qDebug()<<chunkSize;
    qDebug()<<chunkFormat;
    if(chunkFormat == "WAVE")
    {qDebug()<<"That's a fucking WAVE file ! ";}
    else{
        qDebug()<<
               "Pick another file ";
    }
    //subchunk2

    subchunk1ID.append(bytes, 16);
    subchunk1ID.remove(0,12);
    //the space here is important after "fmt "(4bytes for id)
    if(subchunk1ID=="fmt ")
    {
        qDebug()<<"the file is 'fmt'";
    }
    subchunk1Size   =   ((unsigned char)bytes[19]<<24) | ((unsigned char)bytes[18]<<16)|((unsigned char)bytes[17]<<8)|((unsigned char)bytes[16]);
    audioFormat     =   ((unsigned char)bytes[21]<<8)|((unsigned char)bytes[20]);
    numChannels     =   ((unsigned char)bytes[23]<<8)|((unsigned char)bytes[22]);
    sampleRate      =   ((unsigned char)bytes[27]<<24)|((unsigned char)bytes[26]<<16)|((unsigned char)bytes[25]<<8)|((unsigned char)bytes[24]);
    byteRate        =   ((unsigned char)bytes[31]<<24)|((unsigned char)bytes[30]<<16)|((unsigned char)bytes[29]<<8)|((unsigned char)bytes[28]);
    blockAlign      =   ((unsigned char)bytes[33]<<8)|((unsigned char)bytes[32]);
    bitsPerSample   =   ((unsigned char)bytes[35]<<8)|((unsigned char)bytes[34]);

    //qDebug()<<subchunk1Size;//16 most of the time
    qDebug()<<  "subchunk1 size: "  <<  subchunk1Size ;
    qDebug()<<  "audio format"      <<  audioFormat   ;
    qDebug()<<  "number of chanels:"<<  numChannels   ;
    qDebug()<<  "sampling rate: "   <<  sampleRate    ;
    qDebug()<<  "byteRate:"         <<  byteRate      ;
    qDebug()<<  "block align: "     <<  blockAlign    ;
    qDebug()<<  "bits per sample: " <<  bitsPerSample ;


    //data chunk

    int i=0;
    while(subChunk2ID != "data")//seek until data chunk found
    {
        subChunk2ID[0]=(bytes[36+i]);
        subChunk2ID[1]=(bytes[37+i]);
        subChunk2ID[2]=(bytes[38+i]);
        subChunk2ID[3]=(bytes[39+i]);
         subChunk2sz =  ((unsigned char)bytes[43+i]<<24)|((unsigned char)bytes[42+i]<<16)|((unsigned char)bytes[41+i]<<8)|((unsigned char)bytes[40+i]);
        qDebug()<< "chunk ID: " << subChunk2ID;
        qDebug()<< "chunk Size" <<subChunk2sz;
        i++;
    }


    //(i-1) because of post-increment
    bytes.remove(0,44+(i-1));//remove the headers, this is now purely the data;

   // audio_data AD(bytes);
    qDebug()<<"finished copying";
   // snd_output_t *output = NULL;
    numSamples =(subChunk2sz)/4;
    ch0= new unsigned short[numSamples];

    int j =0;
    if(numChannels == 1)
    {
        for(int i=0; i< numSamples; i++)
        {
            j = i*4;
            ch0[i] = ((unsigned char)bytes[j+1]<<8) | ((unsigned char)bytes[j]);
            //little endian 16 =   ((unsigned char)bytes[21]<<8)|((unsigned char)bytes[20])

        }
    }
    if(numChannels ==2)
    {
         ch1= new unsigned short[numSamples];
        for(unsigned int i=0; i<numSamples; i++)
        {
             j=i*4;
            ch0[i] = ((unsigned char)bytes[j+1]<<8) | ((unsigned char)bytes[j]);
            ch1[i]=((unsigned char)bytes[j+3]<<8) | ((unsigned char)bytes[j+2]);
                   //little endian 16 =   ((unsigned char)bytes[21]<<8)|((unsigned char)bytes[20])

        }
    }
}


void audio_data::set_data(QByteArray bytes)
{
    //need to clear data on new file


    //subchunk 1

        riff.append(bytes,4);

//        qDebug()<<(cstemp[0]<<24);

        chunkSize = ((unsigned char)bytes[7]<<24)|((unsigned char)bytes[6]<<16)|((unsigned char)bytes[5]<<8)|((unsigned char)bytes[4]);//make sure unsigned char

        chunkFormat.append(bytes, 12);
        chunkFormat.remove(0,8);

    qDebug()<<riff;
    qDebug()<<chunkSize;
    qDebug()<<chunkFormat;
    if(chunkFormat == "WAVE")
    {qDebug()<<"That's a fucking WAVE file ! ";}
    else{
        qDebug()<<
               "Pick another file ";
    }
    //subchunk2

    subchunk1ID.append(bytes, 16);
    subchunk1ID.remove(0,12);
    //the space here is important after "fmt "(4bytes for id)
    if(subchunk1ID=="fmt ")
    {
        qDebug()<<"the file is 'fmt'";
    }
    subchunk1Size = ((unsigned char)bytes[19]<<24) | ((unsigned char)bytes[18]<<16)|((unsigned char)bytes[17]<<8)|((unsigned char)bytes[16]);
    audioFormat =   ((unsigned char)bytes[21]<<8)|((unsigned char)bytes[20]);
    numChannels =   ((unsigned char)bytes[23]<<8)|((unsigned char)bytes[22]);
    sampleRate  =   ((unsigned char)bytes[27]<<24)|((unsigned char)bytes[26]<<16)|((unsigned char)bytes[25]<<8)|((unsigned char)bytes[24]);
    byteRate    =   ((unsigned char)bytes[31]<<24)|((unsigned char)bytes[30]<<16)|((unsigned char)bytes[29]<<8)|((unsigned char)bytes[28]);
    blockAlign=     ((unsigned char)bytes[33]<<8)|((unsigned char)bytes[32]);
    bitsPerSample=  ((unsigned char)bytes[35]<<8)|((unsigned char)bytes[34]);

    //qDebug()<<subchunk1Size;//16 most of the time
    qDebug()<<  "subchunk1 size: "  <<  subchunk1Size ;
    qDebug()<<  "audio format"      <<  audioFormat   ;
    qDebug()<<  "number of chanels:"<<  numChannels   ;
    qDebug()<<  "sampling rate: "   <<  sampleRate    ;
    qDebug()<<  "byteRate:"         <<  byteRate      ;
    qDebug()<<  "block align: "     <<  blockAlign    ;
    qDebug()<<  "bits per sample: " <<  bitsPerSample ;


    //data chunk

    int i=0;
    while(subChunk2ID != "data")//seek until data chunk found
    {
        subChunk2ID[0]=(bytes[36+i]);
        subChunk2ID[1]=(bytes[37+i]);
        subChunk2ID[2]=(bytes[38+i]);
        subChunk2ID[3]=(bytes[39+i]);
         subChunk2sz =  ((unsigned char)bytes[43+i]<<24)|((unsigned char)bytes[42+i]<<16)|((unsigned char)bytes[41+i]<<8)|((unsigned char)bytes[40+i]);
        qDebug()<< "chunk ID: " << subChunk2ID;
        qDebug()<< "chunk Size" <<subChunk2sz;
        i++;
    }


    //(i-1) because of post-increment
    bytes.remove(0,44+(i-1));//remove the headers, this is now purely the data;

   // audio_data AD(bytes);
    qDebug()<<"finished copying";
   // snd_output_t *output = NULL;
    numSamples =(subChunk2sz)/4;
    ch0= new unsigned short[numSamples];

    int j =0;
    if(numChannels == 1)
    {
        for(int i=0; i< numSamples; i++)
        {
            j = i*4;
            ch0[i] = ((unsigned char)bytes[j+1]<<8) | ((unsigned char)bytes[j]);
            //little endian 16 =   ((unsigned char)bytes[21]<<8)|((unsigned char)bytes[20])

        }
    }
    if(numChannels ==2)
    {
         ch1= new unsigned short[numSamples];
        for(int i=0; i<numSamples; i++)
        {
             j=i*4;
            ch0[i] = ((unsigned char)bytes[j+1]<<8) | ((unsigned char)bytes[j]);
            ch1[i]=((unsigned char)bytes[j+3]<<8) | ((unsigned char)bytes[j+2]);
                   //little endian 16 =   ((unsigned char)bytes[21]<<8)|((unsigned char)bytes[20])

        }
    }
}
unsigned long audio_data::getSampleRate()
{
    return sampleRate;
}

unsigned long audio_data::getNumSamples(){
    return numSamples;
}
unsigned short * audio_data::getStereoLeft()
{
    return ch0;
}
unsigned short * audio_data::getStereoRight()
{
    return ch1;
}
unsigned short * audio_data::getMono()
{
    return ch0;
}
