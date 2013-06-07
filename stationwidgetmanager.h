#ifndef STATIONWIDGETMANAGER_H
#define STATIONWIDGETMANAGER_H

#include <QObject>

#include <QSlider>
#include <QGroupBox>
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
    QGroupBox* getIndicator() const;
signals:
    
public slots:
private:
    QSlider* m_vol;
    QCheckBox* m_listen;
    QPushButton* m_reply;

    QGroupBox * m_indivator;
};

#endif // STATIONWIDGETMANAGER_H
