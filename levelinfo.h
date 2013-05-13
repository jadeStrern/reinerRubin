#ifndef LEVELINFO_H
#define LEVELINFO_H

#include<QString>
#include<QRegExp>

class LevelInfo
{
public:
    LevelInfo(); // add set with doubles
    void setPeak(const QString& peak);
    void setDecay(const QString& decay);
    void setRMS(const QString& rms);

    double getPeak();
    double getDecay();
    double getRMS();

    double getDeltaRMS();
private:
    QString m_peak;
    QString m_decay;
    QString m_rms;

    double _m_peak;
    double _m_decay;
    double _m_rms;

    double extractDouble(const QString & dstr);

    QRegExp rx;
};

#endif // LEVELINFO_H
