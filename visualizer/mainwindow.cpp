#include "mainwindow.h"
#include "ui_mainwindow.h"



//global for now though im not sure
snd_pcm_t *handle;
snd_pcm_hw_params_t * 	params ;
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
    file.close();
    //qDebug()<< sizeof(bytes);
    //structure which seperates different chunks and all that: audio_data.h
    if(!sound)
        delete sound;
    sound = new audio_data(bytes);
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


void sendSound(audio_data* sound)
{
    int err;
    snd_pcm_sframes_t frames;

    if((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK,0))<0){
        qDebug() <<"playback open error : " << snd_strerror(err);
        exit(EXIT_FAILURE);
    }
//    if((err=snd_pcm_hw_params(handle,params))){
//        qDebug()<<"hw params: "<<snd_strerror(err);
//    };
    if((err = snd_pcm_set_params(handle,
                                 SND_PCM_FORMAT_S16_LE,
                                 SND_PCM_ACCESS_RW_INTERLEAVED,
                                 sound->getNumChannels(),//this here is the # of channels
                                 sound->getSampleRate(),
                                 1,
                                 500000))<0){
        qDebug() <<"playback open error : " << snd_strerror(err);
        exit(EXIT_FAILURE);
    }

    qDebug()<<"sizeof left buff" <<sizeof(sound->getAudio());

    //plays file
        frames= snd_pcm_writei(handle, sound->getAudio(), sound->getNumSamples());
        if(frames<0)
            frames = snd_pcm_recover(handle,frames,0);
        if(frames<0)
        {
            qDebug()<<"sbd_pcm_writei failed" ; //snd_sterror(frames)

        }
        if(frames>0 && frames< sound->getNumSamples())
        {
            qDebug()<<"Short write (expected  "<<sizeof(sound->getAudio()) << "wrote" << frames;
        }

    snd_pcm_close(handle);
}
void MainWindow::on_playButton_clicked()
{
    std::thread play(sendSound, sound);
    play.detach();
}

void MainWindow::on_pauseButton_clicked()
{
    int err;
    if((err= snd_pcm_pause(handle,1))){//0 == resume, 1==pause
        qDebug() <<"pause Error: "<< snd_strerror(err);
    }
}
