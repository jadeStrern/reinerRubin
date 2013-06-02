    #ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include <QGlib/Connect>
#include <QGlib/Error>

#include <QGst/Init>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Pad>
#include <QGst/Structure>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Utils/ApplicationSource>

#include "levelinfo.h"

// sound -> rtcpudpsink -> reinerRubin -> out

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = 0);

    void setPort(int port);
    int getPort() const;

    void play();
    void stop();

    int getVolume();
    void setVolume(double vol);

    bool getMute();
    void setMute(bool mute);

    bool getLevel();
    long getTimeStamp() const;

signals:

public slots:
    private:
    void init();
    void getMessage(const QGst::MessagePtr & message);
    QGst::PipelinePtr m_pipelineIn;
    QGst::ElementPtr volumeOut; // TODO add settings out
    QGst::ElementPtr rtcpudpsink;
    QGst::ElementPtr level;

    LevelInfo levelInfo;

};

#endif // PLAYER_H
