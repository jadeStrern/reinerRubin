#include "playermanager.h"
#include <QTimer>

PLayerManager::PLayerManager(QObject *parent) :
    QObject(parent)
{
    m_player =  new Player();
    m_player->setMute(true);
    m_widgets = new StationWidgetManager();
    m_widgets->getVolSlider()->setSliderPosition(toSlider(m_player->getVolume()));
    m_widgets->getListenCheckBox()->setChecked(!m_player->getMute());

    QObject::connect(m_widgets->getVolSlider(), SIGNAL(valueChanged(int)),
                     this,                      SLOT(changeVolume(int)));

    QObject::connect(m_widgets->getListenCheckBox(), SIGNAL(clicked()),
                     this,                           SLOT(swapMute()));

    QObject::connect(m_widgets->getReplyPushButton(), SIGNAL(clicked()),
                     this,                            SLOT(reply()));
    this->play();
//    QTimer* timer = new QTimer();
//    timer->start(1000);
//    QObject::connect(timer, SIGNAL(timeout()),
//                     this,  SLOT(checkDelta()));
}

void PLayerManager::play() {
    m_player->play();
}

void PLayerManager::changeVolume(int vol) {
    qDebug() << "set volume " << vol;
    m_player->setVolume(toVol(vol));
}

void PLayerManager::swapMute() {
    m_player->setMute(!m_player->getMute());
}

void PLayerManager::setPort(int port) {
    m_player->setPort(port);
}

int PLayerManager::getPort() const {
    return m_player->getPort();
}

void PLayerManager::setStreamSource(const QString &host) {
    m_srcStreamHost = host;
}

QString PLayerManager::getStreamSource() const {
    return m_srcStreamHost;
}


void PLayerManager::reply() {
    qDebug() << "reply to " << m_srcStreamHost << " " << m_player->getPort();
    m_widgets->getIndicator()->setTitle(tr("statusReplied"));
//    m_widgets->getIndicator()->setStyleSheet("background: blue");
    emit replied(m_srcStreamHost, m_srcInPort);
    emit settedActive(this);
}

QString PLayerManager::getStationName() const {
    return m_stationName;
}

void PLayerManager::setStationName(const QString &name) {
    m_stationName = name;
    m_widgets->getListenCheckBox()->setText(m_stationName);
}

void PLayerManager::setSourceInPort(int port) {
    m_srcInPort = port;
}

int PLayerManager::getSourceInPort() const {
    return m_srcInPort;
}

StationWidgetManager* PLayerManager::getWidgets() {
    return m_widgets;
}

void PLayerManager::checkDelta() {
    qDebug() << "delta " << (double)m_player->getLevel();
}

int PLayerManager::toSlider(double vol) {
    return (int)vol*sliderGstreamerCof;
}

double PLayerManager::toVol(int vol) {
    return (double)vol/sliderGstreamerCof;
}
