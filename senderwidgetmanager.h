#ifndef SENDERWIDGETMANAGER_H
#define SENDERWIDGETMANAGER_H

#include <QObject>

#include <QSlider>

class SenderWidgetManager : public QObject
{
    Q_OBJECT
public:
    explicit SenderWidgetManager(QObject *parent = 0);
    QSlider* getVolSlider();
    void setVolSlider(QSlider* volSlider);
signals:
    
public slots:
private:
    QSlider* m_volume;
    
};

#endif // SENDERWIDGETMANAGER_H
