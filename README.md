# uart_server
# by CZJ
a server for serial to net.

This project is a serial port server project,and  it is use to get massage from remote uart devices.
This project is and embedded linux project. The platform is smart4418, and I use wk2114 to expand serial port.
Beside uart communication, the project also has the function to control remote IO and have some special funtion than I want.
The project mainly include gpio drivers which is based on the linux platform driver.
platform include the drivers. Junction_driver.c is the driver and junction_dev0.c ~ junction_dev4.c is 5 devices.
Uart_server is for uart communication through net, and junction_server is for junction contron through ethernet.