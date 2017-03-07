#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFS_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/felix/mp3Viz");
    qDebug()<<"opening file"<<filename;
    //file is the mp3 file (hopefully)
    QFile file(filename);
    //if we can't read file, return
    if(!file.open(QIODevice::ReadOnly)) return;

    QByteArray bytes = file.readAll();
    //file.close();
//    unsigned char *bytes = file.readAll();
    QByteArray riff,chunkFormat;
    quint32 chunkSize;
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
    QByteArray subchunk1ID;
    //all of these are little endian
    quint32 subchunk1Size, sampleRate, byteRate;
    quint16 audioFormat,numChannels, blockAlign,bitsPerSample;
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

    if(audioFormat != 1)
    {
        //if not PCM then there is more to the header.
        ui->error_lbl->setText("'I can only do PCM type for now'");
    }
    //data chunk
    QByteArray subChunk2ID;
    quint32  subChunk2sz;

    subChunk2ID[0]=(bytes[36]);
    subChunk2ID[1]=(bytes[37]);
    subChunk2ID[2]=(bytes[38]);
    subChunk2ID[3]=(bytes[39]);
    subChunk2sz =  ((unsigned char)bytes[43]<<24)|((unsigned char)bytes[42]<<16)|((unsigned char)bytes[41]<<8)|((unsigned char)bytes[40]);
    qDebug()<< subChunk2ID;
    qDebug()<< subChunk2sz;

    bytes.remove(0,44);//remove the headers, this is now purely the data;


    // do something..

      //QAudioOutput* audio;
    QAudioFormat format;
    format.setSampleRate(sampleRate);
    format.setChannelCount(numChannels);
    format.setSampleSize(bitsPerSample);
    format.setCodec("audio/PCM");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    //QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
//    if(!info.isFormatSupported(format))
//    {
//        ui->error_lbl->setText("'not supported bud'");
//        return;
//    }
    //audio = new QAudioOutput(format,this);
    //connect(audio, SIGNAL(stateChanged(QAudio::State)),this, SLOT(handleStateChanged(QAudio::State, &file, &audio)));
    //audio->start(&file);
    //uses my filestructure to creade the left and right channels
   // audio_data AD(bytes);
    qDebug()<<"finished copying";
   // snd_output_t *output = NULL;
    qDebug()<<(subChunk2sz)/4;

    unsigned short leftBuffer[(subChunk2sz)/4];
    for(int i=0; i<subChunk2sz/4; i++)
    {
       int j = i*4;
        leftBuffer[i] = ((unsigned char)bytes[j+1]<<8) | ((unsigned char)bytes[j]);

    }
    int err;

    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
//    unsigned char buffer[32*1024];
//                for (i = 0; i < sizeof(buffer); i++)
//                        buffer[i] = random() & 0xff;
    if((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK,0))<0){
        qDebug() <<"playback open error : " << snd_strerror(err);
        exit(EXIT_FAILURE);
    }
    if((err = snd_pcm_set_params(handle,
                                 SND_PCM_FORMAT_S16_LE,
                                 SND_PCM_ACCESS_RW_INTERLEAVED,
                                 1,//this here is the # of channels, I'm gonna need to make this stereo but right now I don't know how
                                 sampleRate,
                                 1,
                                 500000))<0){
        qDebug() <<"playback open error : " << snd_strerror(err);
        exit(EXIT_FAILURE);
    }

    qDebug()<<"sizeof left buff" <<sizeof(leftBuffer);

    for(int i=0; i<3;i++)
    {
        frames= snd_pcm_writei(handle, leftBuffer,subChunk2sz/4);
        if(frames<0)
            frames = snd_pcm_recover(handle,frames,0);
        if(frames<0)
        {
            qDebug()<<"sbd_pcm_writei failed" ; //snd_sterror(frames)
            break;
        }
        if(frames>0 && frames<(subChunk2sz/4))
        {
            qDebug()<<"Short write (expected  "<<sizeof(leftBuffer) << "wrote" << frames;
        }
    }
    snd_pcm_close(handle);


}


void MainWindow::handleStateChanged(QAudio::State newState, QFile *file, QAudioOutput *audio)
{
    if(newState == QAudio::IdleState)
    {
        audio->stop();
        file->close();
        delete audio;

    }
    else if(newState == QAudio::StoppedState)
    {
        if(audio->error() != QAudio::NoError)
        {
            qDebug()<<"There's an error ";
        }
    }
}

