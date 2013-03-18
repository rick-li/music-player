#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMidi.h>
#include <QMidiFile.h>
#include <QSettings>
#include "player.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(int argc, char* argv[], QWidget *parent = 0);
  ~MainWindow();
  
private slots:
  void on_actionPlay_triggered();
  void on_actionStop_triggered();
  void on_actionOpen_triggered();
  void on_actionSave_triggered();
  void setSliderValue(int v);


private:
  Ui::MainWindow *ui;
  bool initOK;

  QMidiFile* midiFile;
  Player* player;
  QSettings* appSettings;
  QString midiFileLoc;

  void openMidiFile(QString filename);
};

#endif // MAINWINDOW_H
