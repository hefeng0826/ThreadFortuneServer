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
    void on_clientStateChanged(QString, quint16, QString);
private slots:
    void on_actConnect_toggled(bool arg1);

    void on_actionsend_triggered();

    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;
    FortuneServer *_server;
};

#endif // MAINWINDOW_H
