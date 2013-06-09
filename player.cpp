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
    m_pipelineIn->setProperty("delay", 1005000);

    rtcpudpsink = QGst::ElementFactory::make("udpsrc");
    rtcpudpsink->setProperty("port", 5000);
    rtcpudpsink->setProperty("caps", QGst::Caps::fromString("application/x-rtp,media=(string)audio,clock-rate=(int)8000,encoding-name=(string)SPEEX,payload=(int)110,encoding-params=(string)1"));

    m_pipelineIn->add(rtcpudpsink);

    QGst::ElementPtr bin;

    try {
        bin = QGst::Bin::fromDescription(
            "gstrtpbin do-lost=true latency=1000 ! rtpspeexdepay ! speexdec"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineIn->add(bin);
    rtcpudpsink->link(bin);

    QGst::ElementPtr muxer = QGst::ElementFactory::make("tee");
    QGst::PadPtr muxOutAudio = muxer->getRequestPad("src%d");
    QGst::PadPtr muxOutVAD = muxer->getRequestPad("src%d");


    m_pipelineIn->add(muxer);
    bin->link(muxer);

    QGst::ElementPtr converter1;
    try {
        converter1 = QGst::Bin::fromDescription(
            "queue max-size-buffers=0 max-size-bytes=0 max-size-time=5000000000 leaky=downstream !  audioconvert"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineIn->add(converter1);

    muxOutAudio->link(converter1->getStaticPad("sink"));

    // ******************************************

    QGst::ElementPtr vVADHole;
    try {
        vVADHole = QGst::Bin::fromDescription(
            "queue max-size-buffers=0 max-size-bytes=0 max-size-time=5000000000 leaky=downstream ! vader auto-threshold=true ! fakesink"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineIn->add(vVADHole);

    muxOutVAD->link(vVADHole->getStaticPad("sink"));
    // ******************************************

    volumeOut = QGst::ElementFactory::make("volume"); // TODO settings
    m_pipelineIn->add(volumeOut);
    converter1->link(volumeOut);

    QGst::ElementPtr audioSynk = QGst::ElementFactory::make("autoaudiosink");
    m_pipelineIn->add(audioSynk);
    volumeOut->link(audioSynk);

    m_pipelineIn->bus()->addSignalWatch();
    QGlib::connect(m_pipelineIn->bus(), "message", this, &Player::getMessage);
    QGlib::connect(m_pipelineIn->bus(), "message::buffering", this, &Player::getMessage);
    QGlib::connect(m_pipelineIn->bus(), "message::async-done", this, &Player::getMessage);

}

void Player::play() {
    qDebug() << "start plaing " << m_pipelineIn->setState(QGst::StatePlaying);
    m_pipelineIn->syncStateWithParent();
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
    rtcpudpsink->setProperty("port", port);
}
int Player::getPort() const {
    QGlib::Value port = rtcpudpsink->property("port");
    return port.toInt();
}

bool Player::getLevel() {
    return levelInfo.getAboveStatus();
}

long Player::getTimeStamp() const {
    return levelInfo.getTimestamp();
}

void Player::getMessage(const QGst::MessagePtr &message) {
    QGst::StructurePtr str = message->internalStructure();
    int endq = str->numberOfFields();
    qDebug() << "name " << str->name() << message->typeName();
    switch (message->type()) {
        case QGst::MessageError:
            //check if the pipeline exists before destroying it,
            //as we might get multiple error messages
            qDebug() << "fuck " << message->internalStructure();
            break;
        case QGst::MessageBuffering:
            qDebug() << "MessageBuffering " << str->name();
            for(int it = 0; it != endq; ++it) {
                qDebug() << "param " << str->fieldName(it) << " value " << str->value(str->fieldName(it).toLatin1().data());
            }
            break;
        default:
            if (str->name() == "vader") {
                qDebug() << message->typeName();
                qDebug() << str->name();
                 levelInfo.setAboveStatus(str->value("above").toBool());
                 levelInfo.setTimestamp(str->value("timestamp").toLong());
            }
            break;
    }
}
