#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>

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
    void on_clientStateChanged(QString, quint16, bool, qintptr);
private slots:
    void on_actConnect_toggled(bool arg1);
private:
    Ui::MainWindow *ui;
    FortuneServer *_server;
    QSet<qintptr> _descriptors;
};

#endif // MAINWINDOW_H
