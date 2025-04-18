开发流程：V1~V4keil5上开发

服务器地址：49.232.32.131

V1.0

想出了架构

V2.0.

实现了读取温度

实现了控制风扇转速

V3.0

添加了模式切换按钮。

增加了手动模式，手动按键调节风扇转速。

切换模式使用EXIT外部中断控制。

V4.0

添加了USART发送数据(中断)和接受(查询)数据的功能。(只是尝试使用多种方法，中断的速度最快)

V5.0

使用QT 控制 STM32

QT的界面将显示温度和转速

并且将控制信号通过传给STM32上。

V6.0

编写了并发的服务器，部署在本机的另一台Linux子系统上，使用了Epoll。(学习目的。部署在腾讯云平台)

运行测试样例时。服务器显示：

![image](https://github.com/user-attachments/assets/21d1866f-d48d-453c-91d1-76f146fbe546)

以下是测试结果
![image](https://github.com/user-attachments/assets/1090fe1d-48ef-4798-8e89-8f171b146e3c)

![image](https://github.com/user-attachments/assets/b8ecc8a5-e448-465d-81f3-56088264bc08)




