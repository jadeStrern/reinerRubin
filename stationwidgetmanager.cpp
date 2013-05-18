#include "stationwidgetmanager.h"

StationWidgetManager::StationWidgetManager(QObject *parent) :
    QObject(parent)
{
        m_vol = new QSlider();
        m_vol->setGeometry(QRect(369, 340, 181, 20));
        m_vol->setOrientation(Qt::Horizontal);

        m_reply = new QPushButton(tr("actionReply"));
        m_reply->setMinimumWidth(100);

        m_listen = new QCheckBox("what");

        m_indivator = new QGroupBox(tr("statusNotReplied")); // TODO add
        m_indivator->setFixedSize(100, 30);
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

QGroupBox* StationWidgetManager::getIndicator() const {
    return m_indivator;
}
