#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QLabel>
#include <QTcpSocket>

#include <QTcpServer>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readTcpData(); // 读取 TCP 数据
    void sendTcpRequest();// 发送TCP数据
private slots:
    void readData();  // 读取数据的槽函数
    void sendHexData();  // 发送数据槽函数

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
