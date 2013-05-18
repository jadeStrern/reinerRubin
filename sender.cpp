#include "sender.h"

Sender::Sender(QObject *parent) :
    QObject(parent)
{
    init();
}

void Sender::init() {
    QGst::init();
    m_pipelineOut = QGst::Pipeline::create();

    QGst::ElementPtr rtpbin = QGst::ElementFactory::make("gstrtpbin");
    if (!rtpbin) {
        qFatal("Failed to create gstrtpbin");
    }

    m_pipelineOut->add(rtpbin);

    //audio content
    //sending side
    QGst::ElementPtr audiosrc;

    try {
        audiosrc = QGst::Bin::fromDescription(
            "audiotestsrc ! queue ! audioconvert ! audiorate ! audio/x-raw-int,rate=8000 ! audioconvert"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineOut->add(audiosrc);

    volumeOut = QGst::ElementFactory::make("volume");
    m_pipelineOut->add(volumeOut); // TODO settings

    audiosrc->link(volumeOut);

    QGst::ElementPtr decoder;
    try {
        decoder = QGst::Bin::fromDescription(
            "speexenc vad=false ! rtpspeexpay"
        );
    } catch (const QGlib::Error & error) {
        qCritical() << error;
        qFatal("One ore more required elements are missing. Aborting...");
    }
    m_pipelineOut->add(decoder);
    volumeOut->link(decoder);

    decoder->link(rtpbin, "send_rtp_sink_1");

    rtpudpsink =  QGst::ElementFactory::make("udpsink");
    if (!rtpudpsink) {
        qFatal("Failed to create udpsink. Aborting...");
    }

    rtpudpsink->setProperty("host", "127.0.0.1"); // TODO setings
    rtpudpsink->setProperty("port", 5000); // TODO settings
    m_pipelineOut->add(rtpudpsink);
    rtpbin->link("send_rtp_src_1", rtpudpsink);
}


void Sender::setHost(const QString &host) {
   rtpudpsink->setProperty("host", host);
}

void Sender::setPort(int port) {
    rtpudpsink->setProperty("port", port);
}

void Sender::play() {
    m_pipelineOut->setState(QGst::StatePlaying);
}

void Sender::setVolume(double vol) {
    qDebug() << "vol changed " << vol;
    volumeOut->setProperty("volume", vol);
}
double Sender::getVolume() const {
    QGlib::Value vol = volumeOut->property("volume");
    return vol.get<double>();
}


int Sender::getPort() const {
    QGlib::Value port = rtpudpsink->property("port");
    return port.toInt();
}

QString Sender::getHost() const {
    QGlib::Value host = rtpudpsink->property("host");
    return host.toString();
}
