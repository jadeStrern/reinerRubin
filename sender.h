#ifndef SENDER_H
#define SENDER_H

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

// otosama -> Reiner Ruby -> out -> rozen maidenes
class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = 0);
    void setHost(const QString& host);
    void setPort(int port);
    QString getHost() const;
    int getPort() const;
    void play();

    void setVolume(double vol);
    double getVolume() const;
signals:
    
public slots:

private:
    void init();
    QGst::PipelinePtr m_pipelineOut;
    QGst::ElementPtr volumeOut; // TODO add settings out
    QGst::ElementPtr rtpudpsink;
};

#endif // SENDER_H
