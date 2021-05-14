#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTcpServer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel  *LabListen;//状态栏标签
    QLabel  *LabSocketState;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString getLocalIP();
protected:
    //void closeEvent(QCloseEvent *event);
private slots:
    void onNewConnection();//QTcpServer的newConnection()信号
    void onSocketStateChange (QAbstractSocket::SocketState socketState);
    void onClientConnected() ; //Client Socket connected
    void onClientDisconnected() ; //Client Socket di sconnected
    void onSocketReadyRead() ;//读取
    void on_actStart_clicked();
    void on_actStop_clicked();
    void on_send_clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
