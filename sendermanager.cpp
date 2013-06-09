#include "sendermanager.h"

SenderManager::SenderManager(QObject *parent) :
    QObject(parent)
{
    m_sender = new Sender();
    m_widgets = new SenderWidgetManager();

    m_widgets->getVolSlider()->setSliderPosition(toSlider(m_sender->getVolume()));

    QObject::connect(m_widgets->getVolSlider(), SIGNAL(valueChanged(int)),
                     this,                      SLOT(changeVolume(int)));
}

void SenderManager::setDestination(const QString &host, int port) {
    m_sender->setHost(host);
    m_sender->setPort(port);
}

void SenderManager::changeVolume(int vol) {
    m_sender->setVolume(toVol(vol));
}

void SenderManager::setVolSlider(QSlider *volSlider) {
    QObject::connect(volSlider, SIGNAL(valueChanged(int)),
                     this,      SLOT(changeVolume(int)));
    QObject::disconnect(m_widgets->getVolSlider(), 0, 0, 0);
    m_widgets->setVolSlider(volSlider);
    m_widgets->getVolSlider()->setSliderPosition(toSlider(m_sender->getVolume()));
}

void SenderManager::changeDestination(const QString &host, int port) {
    qDebug() << "change destinatiob slot " << host << " " << port;
    setDestination(host, port);
}

void SenderManager::sendOff() {
    setDestination("", 0);
}

int SenderManager::getDestPort() {
    return m_sender->getPort();
}

QString SenderManager::getDestHost() {
    return m_sender->getHost();
}

void SenderManager::play() {
    m_sender->play();
}


int SenderManager::toSlider(double vol) {
    return (int)vol*sliderGstreamerCof;
}

double SenderManager::toVol(int vol) {
    return (double)vol/sliderGstreamerCof;
}
