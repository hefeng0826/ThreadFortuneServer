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
    void on_clientStateChanged(QString, quint16, bool, qint32);
private slots:
    void on_actConnect_toggled(bool arg1);
    void on_comboBoxRange_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    FortuneServer *_server;
    QSet<qintptr> _descriptors;
};

#endif // MAINWINDOW_H
