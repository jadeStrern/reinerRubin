#include <QTimer>
#include <QLabel>
#include <QGridLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "playermanager.h"
#include "sendermanager.h"
#include "playersgroup.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PlayersGroup* playersGr = new PlayersGroup();

    SenderManager* snm = new SenderManager();
    snm->setVolSlider(ui->horizontalSlider);
    snm->setDestination("", 0);

    QSettings m_settings("/tmp/config.ini", QSettings::IniFormat);
    m_settings.setIniCodec("UTF-8");

    m_settings.beginGroup("stations");
    const QStringList stations = m_settings.allKeys();
    QStringList::ConstIterator it, end;


    ui->scrollAreaWidgetContents->setContentsMargins(10, 0, 10, 10);
    QGridLayout* gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    gridLayout->setVerticalSpacing(20);

    int pos = 0;

    for(it = stations.begin(), end = stations.end(); it != end; ++it) {
        QList<QVariant> stationParam = m_settings.value(*it).toList();
        qDebug() << stationParam;

        PLayerManager* plm = new PLayerManager();

        plm->setStreamSource(stationParam.at(0).toString());
        plm->setPort(stationParam.at(1).toInt());
        plm->setSourceInPort(stationParam.at(2).toInt());
        plm->setStationName(stationParam.at(3).toString()); // TODO check

        gridLayout->addWidget(plm->getWidgets()->getVolSlider(), pos, 0);
        gridLayout->addWidget(plm->getWidgets()->getListenCheckBox(), pos, 1);
        gridLayout->addWidget(plm->getWidgets()->getReplyPushButton(), pos, 2);
        gridLayout->addWidget(plm->getWidgets()->getIndicator(), pos, 3);

        playersGr->addPlayer(plm);
        plm->play();
        ++pos;
    }
    QObject::connect(playersGr, SIGNAL(setAtcived(QString,int)),
                     snm,       SLOT(changeDestination(QString,int)));

    QObject::connect(playersGr, SIGNAL(answeredOff()),
                     snm, SLOT(sendOff()));

    snm->play();
//    snm->setDestination("", 0);
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
}

void MainWindow::testLevel() {
//    qDebug() << "level " << pl->getLevel();
}



MainWindow::~MainWindow()
{
    delete ui;
}
