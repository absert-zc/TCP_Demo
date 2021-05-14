#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include<QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowTitle("TCP  Server----本机IP:");
    LabListen=new QLabel ("监听状态:");
    LabListen->setMinimumWidth(150);
    statusBar()->addWidget(LabListen);
    LabSocketState=new QLabel ("Socket状态: ");
    LabSocketState->setMinimumWidth(200);
    statusBar()->addWidget (LabSocketState);

    QString localIP = getLocalIP();//本机IP
    this->setWindowTitle ("Sever----本机IP: "+localIP);
    ui->comboIP->addItem(localIP);
    tcpServer = new QTcpServer(this) ;
    connect (tcpServer, SIGNAL(newConnection()),this, SLOT(onNewConnection()));
    connect(ui->Quit,&QPushButton::clicked,[=](){
         this->close();
    });
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

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actStart_clicked()
{
    QString IP=ui->comboIP->currentText();//IP地址
    quint16 port=ui->spinPort->value() ; //端口
    QHostAddress addr(IP);
    tcpServer->listen(addr,port);//开始监听
    ui->plainTextEdit->appendPlainText ("**开始监听...");
    ui->plainTextEdit->appendPlainText ("**服务器地址: "+tcpServer->serverAddress().toString());
    ui->plainTextEdit->appendPlainText ("**服务器端口:"+QString::number (tcpServer->serverPort()));
    ui->actStart->setEnabled(false) ;
    ui->actStop->setEnabled(true);
    LabListen->setText ("监听状态:正在监听");
}

void MainWindow::onNewConnection()
{
    tcpSocket = tcpServer->nextPendingConnection(); //获取socket
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(onClientConnected()));
    onClientConnected();
    connect(tcpSocket, SIGNAL (disconnected()),this, SLOT(onC1ientDisconnected()));
    connect(tcpSocket, SIGNAL (stateChanged(QAbstractSocket::SocketState)),this,SLOT (onSocketStateChange (QAbstractSocket::SocketState)));
    onSocketStateChange(tcpSocket->state());
    connect(tcpSocket, SIGNAL (readyRead()),this, SLOT(onSocketReadyRead()));
}

void MainWindow::onClientConnected()
{
    ui->plainTextEdit->appendPlainText ("**client socket connected") ;
    ui->plainTextEdit->appendPlainText ("**peer address:"+tcpSocket->peerAddress().toString());
    ui->plainTextEdit->appendPlainText ("**peer port:"+QString::number(tcpSocket->peerPort()));
}

void MainWindow::onClientDisconnected()
{
    ui->plainTextEdit->appendPlainText ("**client socket disconnected") ;
    tcpSocket->deleteLater();
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




void MainWindow::on_actStop_clicked()
{
    if (tcpServer->isListening())
    {
        tcpServer->close() ;//停止监听
        ui->actStart->setEnabled(true) ;
        ui->actStop->setEnabled(false) ;
        LabListen->setText ("监听状态:已停止监听") ;
    }
}

void MainWindow::on_send_clicked()
{//发送一行字符串，以换行符结束
    QString msg=ui->editMsg->text();
    ui->plainTextEdit->appendPlainText("[out] "+msg);
    ui->editMsg->clear() ;
    ui ->editMsg->setFocus();
    QByteArray str=msg.toUtf8() ;
    str.append('\n');//添加一个换行符
    tcpSocket->write(str) ;
}

void MainWindow::onSocketReadyRead()
{
    while(tcpSocket->canReadLine())
        ui->plainTextEdit->appendPlainText("[in]"+tcpSocket->readLine());
}


