#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <cstdio>
#include <QBitArray>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <alsa/asoundlib.h>
#include <audio_data.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openFS_clicked();
    void handleStateChanged(QAudio::State, QFile *file, QAudioOutput *audio);

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
