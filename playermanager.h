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
    // audio source port
    void setPort(int port);
    int getPort() const;
    // port 2 answer
    void setSourceInPort(int port);
    int getSourceInPort() const;
    // host 2 answeer
    void setStreamSource(const QString& host);
    QString getStreamSource() const;

    void setStationName(const QString& name);
    QString getStationName() const;

    void play();
    StationWidgetManager* getWidgets();
signals:
    void replied(const QString& host, int port);
    void settedActive(PLayerManager * plm);
public slots:
    void changeVolume(int vol);
    void swapMute(); // maybe do on off

    void checkDelta();
private slots:
    void reply();
private :
    StationWidgetManager* m_widgets;
    Player* m_player;
    // path 2 answer
    QString m_srcStreamHost;
    int m_srcInPort;
    // just inidificator
    QString m_stationName;

    static const int sliderGstreamerCof = 10;
    double toVol(int vol);
    int toSlider(double vol);

};

#endif // PLAYERMANAGER_H
