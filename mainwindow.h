#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FortuneServer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void on_newClientConnected();
private slots:
    void on_actConnect_toggled(bool arg1);

private:
    Ui::MainWindow *ui;
    FortuneServer *_server;
};

#endif // MAINWINDOW_H
