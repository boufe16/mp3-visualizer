#include "mainwindow.h"
#include "ui_mainwindow.h"


//global for now though im not sure
snd_pcm_t *handle;
//this is the playback stream
snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
//pcm device name
snd_pcm_hw_params_t * 	hwparams ;
char *pcm_name; //not sure if this is better than "default" ? ?

void aSetup(audio_data* sound){



    //pcm_name = strdup("plughw:0,0");//declare name
    pcm_name = "default";
    snd_pcm_hw_params_alloca(&hwparams);//allocate hardware param struct on the stack

    if (snd_pcm_open(&handle, pcm_name, stream, 0) < 0) {
        qDebug()<< "Error opening PCM device "<< pcm_name;
       }
    if(snd_pcm_hw_params_any(handle, hwparams)<0){
        qDebug()<<"Can not configure this PCM device "<< pcm_name;
    }
    // int dir;
    unsigned int exact_rate = sound->getSampleRate();
    //snd_pcm_hw_params_set_rate_near(handle,hwparams,&exact_rate,&dir);
    int periods =2;
    unsigned int rate = sound->getSampleRate();
    qDebug()<< sound->getSampleRate();
    snd_pcm_uframes_t periodsize=8192;
    if(snd_pcm_hw_params_set_access(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED)<0)
    {
        qDebug()<<"error setting access" << stderr;
    }
    if(snd_pcm_hw_params_set_format(handle, hwparams, SND_PCM_FORMAT_S16_LE)<0)
    {
        qDebug()<<"error setting access" << stderr;
    }
    if(snd_pcm_hw_params_set_rate_near(handle,hwparams,&exact_rate,0)<0)
    {
        qDebug()<<"error setting rate "<< stderr;
    }
    if(rate!= exact_rate)
    {
        qDebug()<<"rate not supported by hardware"<<rate<< "instead:: " <<exact_rate;
    }
    if(snd_pcm_hw_params_set_channels(handle, hwparams,sound->getNumChannels())<0)
    {
        qDebug()<< "error setting channels";
    }
    if(snd_pcm_hw_params_set_periods(handle, hwparams, periods, 0)<0)
    {
        qDebug()<<"Error setting periods" ;
    }
    if(snd_pcm_hw_params_set_buffer_size(handle, hwparams,(periodsize*periods)>>2)<0)
    {
        qDebug()<<"error setting buffersize "<<stderr;
    }
    if(snd_pcm_hw_params(handle, hwparams)<0)
    {
        qDebug()<<"error setting HW params"<<stderr;
    }
}
void startAudio(audio_data* sound)
{
    int pcmreturn;
    while ((pcmreturn = snd_pcm_writei(handle, sound->getAudio(), sound->getNumSamples())) < 0) {
        snd_pcm_prepare(handle);
       //fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
    }

    snd_pcm_drop(handle);


   //run this on audio thread
//   while(shared_data->do_audio) {
//         process_buffer(buf, bufsize);
//         while ((pcm_return = snd_pcm_writei(pcm_handle, buf, bufsize)) < 0) {
//             snd_pcm_prepare(pcm_handle);
//             fprintf(stderr, "xrun !\n");
//         }
//     }
}

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
    ui->songTitle->setText(filename);
    //file is the mp3 file (hopefully)
    QFile file(filename);
    //if we can't read file, return
    if(!file.open(QIODevice::ReadOnly)) return;
    QByteArray bytes = file.readAll();
    file.close();
    //qDebug()<< sizeof(bytes);
    //structure which seperates different chunks and all that: audio_data.h
    if(!sound)
        delete sound;
    sound = new audio_data(bytes);
    aSetup(sound);
    std::thread play(startAudio,sound);
    play.detach();//run play then pause

//   // delete bytes;
//    QGraphicsScene *scene = new QGraphicsScene;
//    int j=0;
//    for(int i=1; i< sound->getNumSamples()/10; i+=4)
//    {
//        scene->addLine(j, bytes[i],j+1,bytes[i]+1);
//         scene->addLine(j, bytes[i+1],j+1,bytes[i+1]+1);
//        if(i%5==0)
//        {
//            j++;
//        }
//    }
//    //graphicsView.show();
//    ui->vision->setScene(scene);
//    ui->vision->update();
}

void MainWindow::on_playButton_clicked()
{
    int err;
    //std::thread play(sendSound, sound);
    //play.detach();
    if((err =snd_pcm_pause(handle,0))){//0 == resume, 1==pause
        qDebug() <<"pause Error: "<< snd_strerror(err);

    }

}

void MainWindow::on_pauseButton_clicked()
{
    int err;
    if((err= snd_pcm_pause(handle,1))){//0 == resume, 1==pause
        qDebug() <<"pause Error: "<< snd_strerror(err);
    }
}

void MainWindow::on_stopButton_clicked()
{
    snd_pcm_drop(handle);
    ui->songTitle->setText("Select a song");
}
