#include "levelinfo.h"

#include<QStringList>
#include<QDebug>

LevelInfo::LevelInfo()
{
    // TODO make static
    rx = QRegExp("([\\+,\\-]\\d+\\.\\d+)");
}


double LevelInfo::getDecay() {
    return extractDouble(m_decay);
}

double LevelInfo::getPeak() {
    return extractDouble(m_peak);
}

double LevelInfo::getRMS() {
//    m_lastRMS = extractDouble(m_rms);
//    return m_lastRMS;
    return extractDouble(m_rms);
}


void LevelInfo::setPeak(const QString& peak) {
    m_peak = peak;
}

void LevelInfo::setDecay(const QString& decay) {
    m_decay = decay;
}

void LevelInfo::setRMS(const QString& rms) {
    m_rms = rms;
}

double LevelInfo::extractDouble(const QString &dstr) {
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(dstr, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }

    if(list.size() > 1)
        qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!";

    return list.size() == 0 ? 0 : list.at(0).toDouble();
}

double LevelInfo::getDeltaRMS() {
//    double rms = extractDouble(m_rms);
//    double res = qAbs(rms - m_lastRMS);
//    m_lastRMS = rms;
    return qAbs(getPeak() - getRMS());
}
