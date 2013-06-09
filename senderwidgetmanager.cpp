#include "senderwidgetmanager.h"

SenderWidgetManager::SenderWidgetManager(QObject *parent) :
    QObject(parent)
{ // TODO position
    m_volume = new QSlider();
    m_volume->setGeometry(QRect(369, 340, 181, 20));
    m_volume->setOrientation(Qt::Horizontal);
}

QSlider* SenderWidgetManager::getVolSlider() {
    return m_volume;
}

void SenderWidgetManager::setVolSlider(QSlider *volSlider) {
    delete m_volume;
    m_volume = volSlider;
}
