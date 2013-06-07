#ifndef PLAYERSGROUP_H
#define PLAYERSGROUP_H

#include <QObject>

#include <QList>

#include "playermanager.h"

class PlayersGroup : public QObject
{
    Q_OBJECT

public:
    explicit PlayersGroup(QObject *parent = 0);
    void addPlayer(PLayerManager* plm);
    
signals:
    void setAtcived(const QString& host, int port); // if changed current
    void answeredOff(); // if changed current
public slots:
    void setActive(PLayerManager* plm);
private:
    QList<PLayerManager*>* m_players;
    PLayerManager* m_answered;
};

#endif // PLAYERSGROUP_H
