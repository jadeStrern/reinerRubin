#ifndef SENDERMANAGER_H
#define SENDERMANAGER_H

#include <QObject>

#include "sender.h"
#include "senderwidgetmanager.h"


#include <QSlider>

class SenderManager : public QObject
{
    Q_OBJECT
public:
    explicit SenderManager(QObject *parent = 0);
    void setDestination(const QString& host, int port);
    void setVolSlider(QSlider* volSlider);
signals:
    
public slots:
    void changeVolume(int vol);
    void changeDestination(const QString& host, int port);

private:
    Sender* m_sender;
    SenderWidgetManager* m_widgets;

    static const int sliderGstreamerCof = 10;
    double toVol(int vol);
    int toSlider(double vol);
};

#endif // SENDERMANAGER_H
