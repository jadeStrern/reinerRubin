#ifndef STATIONWIDGETMANAGER_H
#define STATIONWIDGETMANAGER_H

#include <QObject>

#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QAbstractButton>

class StationWidgetManager : public QObject
{
    Q_OBJECT
public:
    explicit StationWidgetManager(QObject *parent = 0);
    QSlider* getVolSlider();
    QCheckBox* getListenCheckBox();
    QPushButton* getReplyPushButton();
signals:
    
public slots:
private:
    QSlider* m_vol;
    QCheckBox* m_listen;
    QPushButton* m_reply;
};

#endif // STATIONWIDGETMANAGER_H
