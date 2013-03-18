#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMidi.h>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>

MainWindow::MainWindow(int argc, char* argv[], QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  appSettings = new QSettings("duosuccess","duoPlayer");
  ui->actionPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
  ui->actionStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
  ui->actionOpen->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
  ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));


  midiFile = 0;
  player = 0;
  midiFileLoc = "";
  QMap<QString,QString> outDev = QMidi::outDeviceNames();
  QStringList ids = outDev.keys();

  foreach(QString id,ids)
  {
      qDebug() << "\n";
      qDebug() << " key " << id << " value: " << outDev.value(id);
  }
  if(ids.length() > 0){
      // default choose 0 device;
      QMidi::closeMidiOut();
      QMidi::initMidiOut(0);
      //openMidiFile("C:/apps/winner-takes-it-all.mid");

  }else{
      //alert user that midi is not supported in system.
      QMessageBox mBox(this);
      mBox.setText("錯誤，無法找到midi設備");
      mBox.exec();
  }

  for(int i = 1;i<argc;i++) {
      QString s = QString::fromUtf8(argv[i]);
      QFile f(s);
      if(f.exists() == true) { openMidiFile(QString(argv[i])); break; }
  }

}

MainWindow::~MainWindow()
{
  delete ui;


}
void MainWindow::setSliderValue(int v)
{
  qDebug() << "slider value is " << v;
}

void MainWindow::on_actionPlay_triggered()
{

  if(!midiFile) { return; }
     if(player) { return; }
     qDebug() << "play";
     player = new Player(midiFile, 0);
     player->moveToThread(player);
     ui->songSlider->setValue(0);
     ui->songSlider->setMaximum(midiFile->events().last()->tick());
     connect(player,SIGNAL(finished()),this,SLOT(on_actionStop_triggered()));
     connect(player,SIGNAL(tickChanged(qint32)),ui->songSlider,SLOT(setValue(int)));
//     connect(player,SIGNAL(tickChanged(qint32)),this,SLOT(setSliderValue(int)));
     player->start();
}

void MainWindow::on_actionStop_triggered()
{
  qDebug() << "stop";
  if(!player) { return; }

  player->stop();
  player->wait();
  delete player;
  player = 0;
  QMidi::outStopAll();
  ui->songSlider->setValue(0);
}
void MainWindow::on_actionSave_triggered()
{
  qDebug() << "actionSave";
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QVariant s = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#else
    QVariant s = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#endif
    QString f = QFileDialog::getSaveFileName(this,tr("Save file..."),
                                             QCoreApplication::applicationDirPath(),
                                             tr("BAT files (*.bat)"));
    if(!f.isEmpty()) {
      //save to file
        QFile batFile(f);
        batFile.open(QIODevice::WriteOnly);
        QString msg = "start " + QCoreApplication::applicationFilePath() + " " + midiFileLoc;
        qDebug() << "bat file content is : "+ msg;
        const char* charMsg = msg.toUtf8().constData();
        batFile.write(charMsg, qstrlen(charMsg));
        batFile.close();

    }else{
      return;

     }

}
void MainWindow::on_actionOpen_triggered()
{
  qDebug() << "actionOpen new";

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QVariant s = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#else
    QVariant s = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#endif
    QString f = QFileDialog::getOpenFileName(this,tr("Open file..."),
                                             appSettings->value("LastFileDlgLoc",s).toString(),
                                             tr("MIDI files (*.mid *.midi)"));
    if(!f.isEmpty()) { openMidiFile(f); }
    else { return; }
}
void MainWindow::openMidiFile(QString filepath)
{
   qDebug() << "open midi file";
  qDebug() << "last index "+filepath.lastIndexOf("/");
  QString filename = filepath.right((filepath.length() - filepath.lastIndexOf("/"))-1);
  qDebug() << "file name is "+filename;
  setWindowTitle("多成中醫 - "+filename);
  midiFileLoc = filepath;

  qDebug() << "Open midi file: " << filepath;
//  if(player) { on_actionStop_triggered(); }
  if(midiFile) { delete midiFile; }
  midiFile = new QMidiFile();
  midiFile->load(filepath);
  on_actionPlay_triggered();
}
