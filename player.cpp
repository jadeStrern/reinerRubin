#include "player.h"

#include <QRegExp>
#include <QStringList>

Player::Player(QObject *parent) :
    QObject(parent)
{
    init();
}

void Player::init() {
    levelInfo = LevelInfo();

    QGst::init();
    m_pipelineIn = QGst::Pipeline::create();

    rtcpudpsink = QGst::ElementFactory::make("udpsrc");
//    rtcpudpsink->setProperty("sync", false);
//    rtcpudpsink->setProperty("async", false);
    rtcpudpsink->setProperty("caps", QGst::Caps::fromString("application/x-rtp,media=(string)audio, clock-rate=(int)8000, encoding-name=(string)SPEEX,payload=(int)110"));
    m_pipelineIn->add(rtcpudpsink);

    QGst::ElementPtr bin;

    try {
        bin = QGst::Bin::fromDescription(
            "rtpspeexdepay ! speexdec ! audioconvert"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineIn->add(bin);
    rtcpudpsink->link(bin);

    volumeOut = QGst::ElementFactory::make("volume"); // TODO settings
    m_pipelineIn->add(volumeOut);
    bin->link(volumeOut);

    level = QGst::ElementFactory::make("level");
    m_pipelineIn->add(level);
    volumeOut->link(level);

    QGst::ElementPtr audioSynk = QGst::ElementFactory::make("autoaudiosink");
    m_pipelineIn->add(audioSynk);
    level->link(audioSynk);

    m_pipelineIn->bus()->addSignalWatch();
    QGlib::connect(m_pipelineIn->bus(), "message", this, &Player::getMessage);
    qDebug() << "init m in " << m_pipelineIn->currentState();
}

void Player::play() {
    m_pipelineIn->setState(QGst::StatePlaying);
}
void Player::stop() {
    m_pipelineIn->setState(QGst::StatePaused);
}


void Player::setVolume(double vol) {
    qDebug() << "set volume player " << vol;
    volumeOut->setProperty("volume", vol);
}

int Player::getVolume() {
    QGlib::Value vol = volumeOut->property("volume");
    return vol.toInt();
}


bool Player::getMute() {
    QGlib::Value vol = volumeOut->property("mute");
    return vol.toBool();
}

void Player::setMute(bool mute) {
    volumeOut->setProperty("mute", mute);
}


void Player::setPort(int port) {
    m_pipelineIn->setState(QGst::StateNull);
    rtcpudpsink->setProperty("port", port);
    m_pipelineIn->setState(QGst::StatePlaying);
}
int Player::getPort() const {
    QGlib::Value port = rtcpudpsink->property("port");
    return port.toInt();
}

double Player::getLevel() {
//    qDebug() << "rms " << levelInfo.getRMS() << " peak " << levelInfo.getPeak() << " decay " << levelInfo.getDecay();
//    qDebug() << "getDelta " << levelInfo.getDeltaRMS();
    return levelInfo.getDeltaRMS();
}

void Player::getMessage(const QGst::MessagePtr &message) {
//    qDebug() << "get message ";
    switch (message->type()) {
    case QGst::MessageError:
        //check if the pipeline exists before destroying it,
        //as we might get multiple error messages
        qDebug() << "fuck ";
        break;
    default:
//        qDebug() << message->typeName();
        QGst::StructurePtr str = message->internalStructure();
        if(str->name() == "level") {    // TODO constants
//            qDebug() << "name is level " << str;

            levelInfo.setDecay(str->value("decay").toString());
            levelInfo.setPeak(str->value("peak").toString());
            levelInfo.setRMS(str->value("rms").toString());

            // XXX

//            qDebug() << "val " << rms.toString();
        }
        break;
    }
}
