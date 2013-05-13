#include "stationwidgetmanager.h"

StationWidgetManager::StationWidgetManager(QObject *parent) :
    QObject(parent)
{
        m_vol = new QSlider();
        m_vol->setGeometry(QRect(369, 340, 181, 20));
        m_vol->setOrientation(Qt::Horizontal);

        m_reply = new QPushButton("reply");

        m_listen = new QCheckBox("what");
}

QSlider* StationWidgetManager::getVolSlider() {
    return m_vol;
}

QPushButton* StationWidgetManager::getReplyPushButton() {
    return m_reply;
}

QCheckBox* StationWidgetManager::getListenCheckBox() {
    return m_listen;
}
