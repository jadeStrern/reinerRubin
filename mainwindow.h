#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QList>
#include <QSettings>

#include "playermanager.h"
#include "sendermanager.h"
#include "playersgroup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void testSlot1();
    void testSlot2();

    void testVolume(int vol);
    void testLevel();

private:
    Ui::MainWindow *ui;

    PlayersGroup* playersGr;
    SenderManager* snm;
    QSettings m_settings;
//    QSettings m_settings;
};

#endif // MAINWINDOW_H
