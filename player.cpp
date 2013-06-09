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
//    rtcpudpsink->setProperty("buffer-size", 100);
//    rtcpudpsink->setProperty("sync", false);
//    rtcpudpsink->setProperty("buffer-size", 100000);
//    rtcpudpsink->setProperty("async", false);
    rtcpudpsink->setProperty("port", 5000);
    rtcpudpsink->setProperty("caps", QGst::Caps::fromString("application/x-rtp,media=(string)audio,clock-rate=(int)8000,encoding-name=(string)SPEEX,payload=(int)110,encoding-params=(string)1"));

    m_pipelineIn->add(rtcpudpsink);

    QGst::ElementPtr bin;

    try {
        bin = QGst::Bin::fromDescription(
//            "rtpspeexdepay ! speexdec ! audioconvert ! vader auto-threshold=true"
//            "gstrtpjitterbuffer latency=100 drop-on-latency=true ! rtpspeexdepay ! speexdec" //! tee name=t ! audioconvert"
//            "gstrtpjitterbuffer latency=300 drop-on-latency=true ! rtpspeexdepay ! speexdec" //! tee name=t ! audioconvert"
//            "rtpspeexdepay ! speexdec" //! tee name=t ! audioconvert"
            "gstrtpbin do-lost=true latency=1000 ! rtpspeexdepay ! speexdec" //! tee name=t ! audioconvert"
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

    QGst::ElementPtr converter1;// = QGst::ElementFactory::make("audioconvert");
    try {
        converter1 = QGst::Bin::fromDescription(
//            "queue max-size-bytes=0 max-size-time=0 ! audioconvert"
//            "queue leaky=downstream ! audioconvert"
            "queue max-size-buffers=0 max-size-bytes=0 max-size-time=5000000000 leaky=downstream !  audioconvert"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineIn->add(converter1);

    muxOutAudio->link(converter1->getStaticPad("sink"));

    // ******************************************

    QGst::ElementPtr vVADHole;// = QGst::ElementFactory::make("audioconvert");
    try {
        vVADHole = QGst::Bin::fromDescription(
//            "queue max-size-bytes=0 max-size-time=0 ! vader auto-threshold=true ! fakesink"
            "queue max-size-buffers=0 max-size-bytes=0 max-size-time=5000000000 leaky=downstream ! vader auto-threshold=true ! fakesink"
//            "vader auto-threshold=true ! fakesink"
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

//    level = QGst::ElementFactory::make("level");
//    m_pipelineIn->add(level);
//    volumeOut->link(level);

    QGst::ElementPtr audioSynk = QGst::ElementFactory::make("autoaudiosink");
    m_pipelineIn->add(audioSynk);
//    level->link(audioSynk);
    volumeOut->link(audioSynk);

    m_pipelineIn->bus()->addSignalWatch();
    QGlib::connect(m_pipelineIn->bus(), "message", this, &Player::getMessage);
    QGlib::connect(m_pipelineIn->bus(), "message::buffering", this, &Player::getMessage);
    QGlib::connect(m_pipelineIn->bus(), "message::async-done", this, &Player::getMessage);


//    qDebug() << "init m in " << m_pipelineIn->currentState() << " and " <<  rtcpudpsink->currentState();
//    qDebug() << "after init " << m_pipelineIn->setState(QGst::StateReady);
//    qDebug() << "after init " << m_pipelineIn->setState(QGst::StatePaused);
//    qDebug() << "init m in " << m_pipelineIn->currentState() << " and " <<  rtcpudpsink->currentState();
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
//    m_pipelineIn->setState(QGst::StateReady);
    volumeOut->setProperty("mute", mute);
//    m_pipelineIn->setState(QGst::StatePlaying);
}


void Player::setPort(int port) {
    rtcpudpsink->setProperty("port", port);
//    rtcpudpsink->setProperty("port", port);
}
int Player::getPort() const {
    QGlib::Value port = rtcpudpsink->property("port");
    return port.toInt();
}

bool Player::getLevel() {
//    qDebug() << "state " << m_pipelineIn->currentState();
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
    //        QGst::StructurePtr str = message->internalStructure();
    //        qDebug() << "wtf " << str->name();
    //        qDebug() << " test " << str->percent("percent");
            if(str->name() == "level") {    // depricated
    //            qDebug() << "name is level " << str;


                // XXX

    //            qDebug() << "val " << rms.toString();
            } else if (str->name() == "vader") {
                qDebug() << message->typeName();
                qDebug() << str->name();
    //            "vader"
    //            "above"  ->  QGlib::Value("gboolean", "FALSE")
    //            "timestamp"  ->  QGlib::Value("guint64", "7990060678")
                 levelInfo.setAboveStatus(str->value("above").toBool());
                 levelInfo.setTimestamp(str->value("timestamp").toLong());
//            } else if(str->name() == "state-changed") {
//                qDebug() << "try hack " << str->value("pending-state").toInt();
//                if(str->value("pending-state").toInt() == 4) {
//                    qDebug() << "lock " << m_pipelineIn->currentState();
//                    m_pipelineIn->setStateLocked(true);
//                }
////                name  "GstMessageState" "state-changed"
////                MessageBuffering  "GstMessageState"
////                param  "old-state"  value  QGlib::Value("GstState", "GST_STATE_NULL")
////                param  "new-state"  value  QGlib::Value("GstState", "GST_STATE_READY")
////                param  "pending-state"  value  QGlib::Value("GstState", "GST_STATE_PLAYING")
////                name  "GstMessageState" "state-change   d
            } else {
                qDebug() << "MessageBuffering " << str->name();
                for(int it = 0; it != endq; ++it) {
                    qDebug() << "param " << str->fieldName(it) << " value " << str->value(str->fieldName(it).toLatin1().data());
                }
            }
            break;
    }
}
