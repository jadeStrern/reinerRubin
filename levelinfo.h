#ifndef LEVELINFO_H
#define LEVELINFO_H

#include<QString>
#include<QRegExp>

class LevelInfo
{
public:
    LevelInfo();

    bool getAboveStatus() const;
    void setAboveStatus(bool above);

    long getTimestamp() const;
    void setTimestamp(long time);

private:
    bool m_above; // true voice || false nope
    long m_time;

};

#endif // LEVELINFO_H
