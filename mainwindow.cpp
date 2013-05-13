#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "playermanager.h"
#include "sendermanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sn = new Sender();
    SenderManager* snm = new SenderManager();
    snm->setVolSlider(ui->horizontalSlider);

    for (int it = 0, end = 2; it != end; ++it) {
        PLayerManager* plm = new PLayerManager();
        plm->setPort(it*2 + 5000);
        plm->setStreamSource("127.0.0.1");


        ui->gridLayout->addWidget(plm->getWidgets()->getVolSlider(), it, 0);
        ui->gridLayout->addWidget(plm->getWidgets()->getListenCheckBox(), it, 1);
        ui->gridLayout->addWidget(plm->getWidgets()->getReplyPushButton(), it, 2);

        QObject::connect(plm, SIGNAL(replied(QString,int)),
                         snm, SLOT(changeDestination(QString,int)));
    }

//    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(testVolume(int)));

}


void MainWindow::testSlot1() {
    qDebug() << "change (drop)!";
//    sn->setDestination("192.168.0.1");
}

void MainWindow::testSlot2() {
    qDebug() << "change (add)!";
//    sn->setDestination("127.0.0.1");
}

void MainWindow::testVolume(int vol) {
    vol = vol == 0 ? 1 : vol;
    qDebug() << "set volume " << vol;
    sn->setVolume(10.0/(100-vol)); // TODO calibrate
}

void MainWindow::testLevel() {
    qDebug() << "level " << pl->getLevel();
}

MainWindow::~MainWindow()
{
    delete ui;
}
