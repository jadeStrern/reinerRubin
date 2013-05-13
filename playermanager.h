#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <QObject>

#include "player.h"
#include "stationwidgetmanager.h"

class PLayerManager : public QObject
{
    Q_OBJECT
public:
    explicit PLayerManager(QObject *parent = 0);

    void setPort(int port);
    void setStreamSource(const QString& host);

    void play();
    StationWidgetManager* getWidgets();
signals:
    void replied(const QString& host, int port);
public slots:
    void changeVolume(int vol);
    void swapMute(); // maybe do on off

    void checkDelta();
private slots:
    void reply();
private :
    StationWidgetManager* m_widgets;
    Player* m_player;
    QString m_srcStreamHost;

    static const int sliderGstreamerCof = 10;
    double toVol(int vol);
    int toSlider(double vol);

};

#endif // PLAYERMANAGER_H
