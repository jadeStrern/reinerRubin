#include "playersgroup.h"

PlayersGroup::PlayersGroup(QObject *parent) :
    QObject(parent)
{
    m_players = new QList<PLayerManager*>();
}

void PlayersGroup::addPlayer(PLayerManager *plm) {
    QObject::connect(plm,  SIGNAL(settedActive(PLayerManager*)), // TODO insert playerMananger
                     this, SLOT(setActive(PLayerManager*)));

    m_players->push_back(plm);
}

void PlayersGroup::setActive(PLayerManager *plm) {
    if(plm != m_answered) {
        QList<PLayerManager*>::iterator it, end;
        m_answered = plm;
        emit(setAtcived(plm->getStreamSource(), plm->getSourceInPort()));
        for(it = m_players->begin(), end = m_players->end(); it != end; ++it) {
            if (plm == (*it)) continue;
            QGroupBox* gr = (*it)->getWidgets()->getIndicator();
            gr->setTitle(tr("statusNotReplied"));
            (*it)->getWidgets()->getReplyPushButton()->setText(tr("actionReply"));
        }
        m_answered->getWidgets()->getReplyPushButton()->setText(tr("actionDropAnsw"));
    } else {

        m_answered->getWidgets()->getReplyPushButton()->setText(tr("actionReply"));
        m_answered->getWidgets()->getIndicator()->setTitle(tr("statusNotReplied"));

        m_answered = NULL;
        emit(answeredOff());
    }
}
