#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTcpSocket *tcpClient; //socket
    QLabel *LabSocketState; //状态栏显示标签
    QString getLocalIP() ;//获取本机IP地址
protected:
    //void closeEvent (QCloseEvent *event) ;
private slots:
    void onConnected() ;
    void onDisconnected();
    void onSocketStateChange (QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();//读取socket传入的数据

    void on_actconnect_clicked();

    void on_actdisconnect_clicked();

    void on_send_clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
