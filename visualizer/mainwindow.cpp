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
    file.close();

    //structure which seperates different chunks and all that: audio_data.h
    if(!sound)
        delete sound;
    sound = new audio_data(bytes);

}


void MainWindow::on_playButton_clicked()
{
    int err;

    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;

    if((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK,0))<0){
        qDebug() <<"playback open error : " << snd_strerror(err);
        exit(EXIT_FAILURE);
    }
    if((err = snd_pcm_set_params(handle,
                                 SND_PCM_FORMAT_S16_LE,
                                 SND_PCM_ACCESS_RW_INTERLEAVED,
                                 1,//this here is the # of channels, I'm gonna need to make this stereo but right now I don't know how
                                 sound->getSampleRate(),
                                 1,
                                 500000))<0){
        qDebug() <<"playback open error : " << snd_strerror(err);
        exit(EXIT_FAILURE);
    }

    qDebug()<<"sizeof left buff" <<sizeof(sound->getMono());

    //plays file
        frames= snd_pcm_writei(handle, sound->getMono(), sound->getNumSamples());
        if(frames<0)
            frames = snd_pcm_recover(handle,frames,0);
        if(frames<0)
        {
            qDebug()<<"sbd_pcm_writei failed" ; //snd_sterror(frames)

        }
        if(frames>0 && frames< sound->getNumSamples())
        {
            qDebug()<<"Short write (expected  "<<sizeof(sound->getMono()) << "wrote" << frames;
        }

    snd_pcm_close(handle);
}
