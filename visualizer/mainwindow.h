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
#include <alsa/asoundlib.h>
#include <audio_data.h>
#include <thread>
#include <QGraphicsView>
#include <QtGui>
#include <QGraphicsScene>

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


    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

private:

    audio_data *sound;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
