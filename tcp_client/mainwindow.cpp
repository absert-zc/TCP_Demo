#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTcpSocket>
#include<QHostAddress>
#include<QHostInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient=new QTcpSocket (this); //创建socket变量
    LabSocketState=new QLabel ("Socket状态: "); //状态栏标签
    LabSocketState->setMinimumWidth (250) ;
    statusBar()->addWidget (LabSocketState) ;
    QString localIP = getLocalIP() ;//本机IP
    this->setWindowTitle ("Client----本机IP: "+localIP) ;
    ui->comboServer->addItem (localIP);
    connect (tcpClient, SIGNAL (connected()) , this, SLOT (onConnected()));
    connect (tcpClient, SIGNAL (disconnected()) , this, SLOT (onDisconnected())) ;
    connect (tcpClient, SIGNAL (stateChanged (QAbstractSocket::SocketState)),this, SLOT (onSocketStateChange (QAbstractSocket: :SocketState))) ;
    connect (tcpClient, SIGNAL (readyRead()),this, SLOT (onSocketReadyRead())) ;
    connect(ui->Quit,&QPushButton::clicked,[=](){
         this->close();
    });
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actconnect_clicked()
{
    QString addr=ui->comboServer->currentText() ;
    quint16 port=ui->spinPort->value();
    tcpClient->connectToHost(addr, port);
}



void MainWindow::on_actdisconnect_clicked()
{
    if (tcpClient->state ()==QAbstractSocket::ConnectedState)
        tcpClient->disconnectFromHost() ;

}
void MainWindow::onConnected()
{
    ui->plainTextEdit->appendPlainText ("**已连接到服务器") ;
    ui->plainTextEdit->appendPlainText ("**peer address:"+tcpClient->peerAddress().toString());
    ui->plainTextEdit->appendPlainText ("**peer port:"+QString:: number (tcpClient->peerPort())) ;
    ui->actconnect->setEnabled(false) ;
    ui->actdisconnect->setEnabled (true) ;
}

void MainWindow::onDisconnected()
{
    ui->plainTextEdit->appendPlainText ("**已断开与服务器的连接") ;
    ui->actconnect->setEnabled (true) ;
    ui->actdisconnect->setEnabled(false) ;
}


void MainWindow::on_send_clicked()
{
    QString msg=ui->editMsg->text() ;
    ui->plainTextEdit->appendPlainText (" [out] "+msg) ;
    ui->editMsg->clear() ;
    ui->editMsg->setFocus() ;
    QByteArray str=msg.toUtf8() ;
    str.append('\n') ;
    tcpClient->write(str) ;

}

QString MainWindow::getLocalIP()
{
    //获取本地的IPv4地址
    QString hostName = QHostInfo::localHostName();//本地主机名
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QString localIP="";
    QList<QHostAddress> addList=hostInfo. addresses();
    if (!addList. isEmpty())
      {  for (int i=0;i<addList.count();i++)
        {
            QHostAddress aHost=addList.at(i);
            if (QAbstractSocket::IPv4Protocol==aHost.protocol())
            {
                localIP=aHost.toString();
                break;
            }
        }
      }
    return localIP;
}


void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
        case QAbstractSocket::UnconnectedState:
          LabSocketState->setText ("scoket状态: UnconnectedState"); break;
        case QAbstractSocket::HostLookupState:
          LabSocketState->setText ("scoket状态: HostLookupState");  break;
        case QAbstractSocket::ConnectingState:
          LabSocketState->setText ("scoket状态: ConnectingState") ; break;
        case QAbstractSocket::ConnectedState:
          LabSocketState->setText ("scoket状态: ConnectedState") ;break;
        case QAbstractSocket::BoundState:
          LabSocketState->setText ("scoket状态: BoundState") ;   break;
        case QAbstractSocket::ClosingState:
          LabSocketState->setText ("scoket状态: ClosingState") ; break;
        case QAbstractSocket::ListeningState:
          LabSocketState->setText ("scoket状态: ListeningState") ; break;

    }
}

void MainWindow::onSocketReadyRead()
{
    while (tcpClient->canReadLine())
       { ui->plainTextEdit->appendPlainText ("[in] "+tcpClient->readLine());}

}
