#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "sender.h"
#include "player.h"

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
    Sender* sn;
    Player* pl;
};

#endif // MAINWINDOW_H
