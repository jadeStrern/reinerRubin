#include "levelinfo.h"

#include<QStringList>
#include<QDebug>

LevelInfo::LevelInfo()
{
    // TODO make static
}



long LevelInfo::getTimestamp() const {
    return m_time;
}

void LevelInfo::setTimestamp(long time) {
    m_time = time;
}

// ********************

bool LevelInfo::getAboveStatus() const {
    return m_above;
}

void LevelInfo::setAboveStatus(bool above) {
    m_above = above;
}
