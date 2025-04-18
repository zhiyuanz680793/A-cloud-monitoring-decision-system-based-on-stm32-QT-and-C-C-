#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QDebug>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置串口
        serialPort = new QSerialPort(this);
        serialPort->setPortName("COM3");  // 根据实际情况修改端口名
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        // 打开串口
        if (!serialPort->open(QIODevice::ReadWrite)) {
            qDebug() << "无法打开串口!";
            return;
        }


        // 连接串口数据可用信号到槽函数
        connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
        connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendHexData);

        // 初始化 TCP 客户端
        socket = new QTcpSocket(this);
        socket->connectToHost("192.168.1.101", 8080);

        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readTcpData);
        connect(ui->tcpSendButton, &QPushButton::clicked, this, &MainWindow::sendTcpRequest);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readData()
{
    // 从串口读取数据
    QByteArray data = serialPort->readAll();
    data.toInt();
    // 将字节数据转换为 ASCII 字符
    //QString receivedData = QString::fromUtf8(data);


    // 更新界面上的 QLabel，显示接收到的数据
    ui->label->setText("接收到的原始AD值：\n" + data);

}
// 发送 UI 输入框里的 Hex 数据

void MainWindow::sendHexData()
{
    // 获取输入框的内容，假设用户输入的是十六进制字符串
    QString inputText = ui->lineEdit->text().remove(' ');

    // 检查输入是否为空
    if (inputText.isEmpty()) {
        qDebug() << "输入为空，未发送";
        return;
    }

    // 将输入文本转换为 QByteArray (假设用户输入的是十六进制字符串)
    QByteArray dataToSend = QByteArray::fromHex(inputText.toUtf8());

    // 如果转换失败，返回
    if (dataToSend.isEmpty()) {
        qDebug() << "转换后的十六进制数据无效";
        return;
    }

    // 调试输出，查看发送的十六进制数据
    qDebug() << "发送数据：" << dataToSend.toHex(' ').toUpper();

    // 通过串口发送数据

    serialPort->write(dataToSend);

}
void MainWindow::readTcpData()
{
    QByteArray data = socket->readAll();
    qDebug() << "收到服务器数据：" << data;
    ui->tcp_show->setText("从服务器收到控制信号为：\n" + data);
}
void MainWindow::sendTcpRequest()// 发送TCP数据
{
    // 获取输入框中的文本
        QString inputText = ui->lineEdit_2->text().trimmed();

        // 检查输入是否为空
        if (inputText.isEmpty()) {
            qDebug() << "输入为空，未发送 TCP 请求";
            return;
        }

        // 将输入内容转换为 QByteArray
        QByteArray dataToSend = inputText.toUtf8();  // 以 UTF-8 格式发送字符串

        // 检查 TCP 连接是否正常
        if (socket->state() == QAbstractSocket::ConnectedState) {
            socket->write(dataToSend);  // 发送数据
            socket->flush();  // 立即发送
            qDebug() << "已通过 TCP 发送请求：" << dataToSend;
        } else {
            qDebug() << "TCP 连接未建立，无法发送请求";
        }
}
